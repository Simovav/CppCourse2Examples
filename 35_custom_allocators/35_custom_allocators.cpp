#include <iostream>
#include <list>
#include <memory>
#include <new>
#include <vector>
#include <cstddef>

// -----------------------------
// Global new/delete overrides
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
// ChunkAllocator using T* vector
// -----------------------------
template<typename T, std::size_t TargetChunkBytes = 4096>//100
class ChunkAllocator {
public:
    using value_type = T;

    ChunkAllocator() = default;

    template<typename U>
    struct rebind { using other = ChunkAllocator<U, TargetChunkBytes>; };

    T* allocate(std::size_t n) {
        std::size_t bytesNeeded = n * sizeof(T);

        if (bytesNeeded > bytesLeftInChunk()) {
            allocateChunk(std::max(bytesNeeded, TargetChunkBytes));
        }
        T* ptr = chunks.back() + chunkOffset;
        chunkOffset += n;
        std::cout << "[ChunkAllocator] Allocated " << n << " elements at " << ptr << "\n";
        return ptr;
    }

    void deallocate(T* p, std::size_t n) {
        std::cout << "[ChunkAllocator] Deallocate called for " << n << " elements at " << p << " (no-op)\n";
    }

    //template<typename U, typename... Args>
    //void construct(U* p, Args&&... args) {
    //    //::new ((void*)p) U(std::forward<Args>(args)...);
    //    new ((void*)p) U(std::forward<Args>(args)...);
    //}
    //
    //template<typename U>
    //void destroy(U* p) {
    //    p->~U();
    //}

    ~ChunkAllocator() {
        for (T* chunk : chunks) {
            //::delete(chunk);
            delete(chunk);
        }
    }

private:
    std::vector<T*> chunks;
    std::size_t chunkOffset = 0;
    std::size_t elementsPerChunk = std::max<std::size_t>(1, TargetChunkBytes / sizeof(T));

    std::size_t bytesLeftInChunk() const {
        if (chunks.empty()) return 0;
        return elementsPerChunk - chunkOffset;
    }

    void allocateChunk(std::size_t chunkSize) {
        T* chunk = static_cast<T*>(::operator new(chunkSize));
        chunks.push_back(chunk);
        chunkOffset = 0;
        elementsPerChunk = chunkSize / sizeof(T);
        std::cout << "[ChunkAllocator] New chunk allocated for " << elementsPerChunk
            << " elements at " << chunk << "\n";
    }
};

// -----------------------------
// Demo
// -----------------------------
int main() {
    std::list<int, ChunkAllocator<int>> myList;
    for (int i = 0; i < 10; ++i) {
        myList.push_back(i);
    }

    std::cout << "List contents: ";
    for (int v : myList) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
