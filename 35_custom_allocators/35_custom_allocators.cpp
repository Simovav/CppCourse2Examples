#include <iostream>
#include <list>
#include <memory>
#include <new>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <utility>

// -----------------------------
// Global new/delete overrides (per tracciare le allocazioni di chunk)
// -----------------------------
void* operator new(std::size_t n) noexcept(false) {
    void* p = std::malloc(n);
    std::cout << "[global new] Allocated " << n << " bytes at address " << p << "\n";
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept {
    std::cout << "[global delete] Deleting memory at address " << p << "\n";
    std::free(p);
}

void operator delete(void* p, std::size_t n) noexcept {
    std::cout << "[global delete(n)] Deleting " << n << " bytes at address " << p << "\n";
    std::free(p);
}

// -----------------------------
// Stato Condiviso per i Chunk (Chunk Pool)
// -----------------------------
struct ChunkPool {
    // Memorizza i blocchi allocati (void* per essere agnostici al tipo T)
    std::vector<void*> chunks;
    std::size_t currentChunkOffset = 0;
    std::size_t currentChunkCapacity = 0; // Capacità in byte del chunk corrente
    void* currentChunkPtr = nullptr;

    // Dimensione del chunk target passata dall'allocatore
    std::size_t TargetChunkBytes;

    ChunkPool(std::size_t targetSize) : TargetChunkBytes(targetSize) {}
};

// Custom Deleter per il ChunkPool: libera tutti i chunk quando l'ultimo shared_ptr viene distrutto
struct PoolDeleter {
    void operator()(ChunkPool* p) const noexcept {
        std::cout << "--- [PoolDeleter] Starting final cleanup of chunks. ---\n";
        for (void* chunk : p->chunks) {
            // Utilizziamo l'operatore delete globale per liberare la memoria (che triggera il nostro override)
            // L'allocazione è stata fatta con '::operator new' (che usa malloc), quindi è corretto usare '::operator delete' (che usa free).
            ::operator delete(chunk);
        }
        delete p; // Dealloca la struttura ChunkPool stessa
        std::cout << "--- [PoolDeleter] Cleanup complete. ---\n";
    }
};

// -----------------------------
// ChunkAllocator
// -----------------------------
template<typename T, std::size_t TargetChunkBytes = 100>
class ChunkAllocator {
public:
    using value_type = T;
    using PoolPtr = std::shared_ptr<ChunkPool>;

    // Costruttore di default: crea un nuovo pool di chunk
    ChunkAllocator()
        : pool(new ChunkPool(TargetChunkBytes), PoolDeleter()) {
    }

    // Costruttore di Rebind: COPIA lo shared_ptr, condividendo lo stato
    template<typename U, std::size_t OtherChunkBytes>
    ChunkAllocator(const ChunkAllocator<U, OtherChunkBytes>& other) noexcept
        : pool(other.getPool()) {
    }

    // Funzione getter per lo shared_ptr, necessaria per il rebind
    PoolPtr getPool() const { return pool; }

    template<typename U>
    struct rebind { using other = ChunkAllocator<U, TargetChunkBytes>; };

    T* allocate(std::size_t n) {
        std::size_t bytesNeeded = n * sizeof(T);

        // Verifica se il chunk corrente ha spazio
        if (bytesNeeded > bytesLeftInChunk()) {
            allocateChunk(std::max(bytesNeeded, pool->TargetChunkBytes));
        }

        // Calcola l'indirizzo e avanza l'offset
        T* ptr = static_cast<T*>(static_cast<void*>(
            static_cast<char*>(pool->currentChunkPtr) + pool->currentChunkOffset));

        pool->currentChunkOffset += bytesNeeded;

        std::cout << "[ChunkAllocator] Allocated " << n << " elements (" << bytesNeeded
            << " B) at " << ptr << "\n";
        return ptr;
    }

    void deallocate(T* p, std::size_t n) {
        // La deallocazione è un no-op per un allocatore a chunking.
        std::cout << "[ChunkAllocator] Deallocate called for " << n << " elements at " << p << " (no-op)\n";
    }

    // Le funzioni construct/destroy non sono necessarie per C++11/17/20, ma buone pratiche le includono se necessarie.
    // L'STL chiama direttamente la new/delete placement.

private:
    PoolPtr pool;

    std::size_t bytesLeftInChunk() const {
        return pool->currentChunkPtr ? (pool->currentChunkCapacity - pool->currentChunkOffset) : 0;
    }

    void allocateChunk(std::size_t chunkSize) {
        // Allocazione della memoria RAW usando l'override globale
        void* chunk = ::operator new(chunkSize);

        pool->chunks.push_back(chunk);
        pool->currentChunkPtr = chunk;
        pool->currentChunkOffset = 0;
        pool->currentChunkCapacity = chunkSize;

        std::cout << "[ChunkAllocator] New chunk allocated for " << chunkSize
            << " bytes at " << chunk << "\n";
    }
};

// -----------------------------
// Operatori di Uguaglianza (necessari per la conformità agli Allocator Requirements)
// -----------------------------
template <typename T1, std::size_t TC1, typename T2, std::size_t TC2>
bool operator==(const ChunkAllocator<T1, TC1>& a, const ChunkAllocator<T2, TC2>& b) noexcept {
    // Due allocatori sono uguali se condividono lo stesso ChunkPool.
    return a.getPool() == b.getPool();
}

template <typename T1, std::size_t TC1, typename T2, std::size_t TC2>
bool operator!=(const ChunkAllocator<T1, TC1>& a, const ChunkAllocator<T2, TC2>& b) noexcept {
    return !(a == b);
}

// -----------------------------
// Demo
// -----------------------------
int main() {
    std::cout << "--- Starting list operations ---\n";

    // TargetChunkBytes = 100, sizeof(std::_List_node<int, ...>) è circa 24-32 byte.
    // Ci aspettiamo 3-4 nodi per chunk.
    std::list<int, ChunkAllocator<int, 100>> myList;

    for (int i = 0; i < 10; ++i) {
        std::cout << "\n[MAIN] Inserting " << i << "...\n";
        myList.push_back(i);
    }

    std::cout << "\n--- List contents: ---\n";
    for (int v : myList) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n--- Destroying list and allocators... ---\n";
    // Quando myList esce dallo scope, il suo allocatore (e le sue copie interne) vengono distrutti.
    // Il PoolDeleter viene chiamato quando l'ultimo shared_ptr va fuori scope, garantendo la pulizia.

    return 0;
}