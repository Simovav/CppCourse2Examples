// set_transparent_demo.cpp
// C++17 - std::set with transparent lookup demo

#include <iostream>
#include <set>
#include <string>
#include <cstdlib>
#include <new>

// -----------------------------
// Clear screen (Windows)
// -----------------------------
void clear_screen() {
    std::system("cls");
}

// -----------------------------
// Global new/delete overrides
// -----------------------------
static bool TRACE_ALLOCATIONS = true;

void* operator new(std::size_t n) noexcept(false) {
    void* p = std::malloc(n);
    if (TRACE_ALLOCATIONS)
        std::cout << "[global new] Allocated " << n << " bytes at address " << p << "\n";
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept {
    if (TRACE_ALLOCATIONS)
        std::cout << "[global delete] Deleting memory at address " << p << "\n";
    std::free(p);
}

void operator delete(void* p, std::size_t n) noexcept {
    if (TRACE_ALLOCATIONS)
        std::cout << "[global delete(n)] Deleting " << n << " bytes at address " << p << "\n";
    std::free(p);
}

// -----------------------------
// Demo: Base std::set lookup (non-transparent)
// -----------------------------
void demo_base_lookup() {
    clear_screen();
    std::cout << "=== demo_base_lookup (non-transparent) ===\n";

    std::set<std::string> names = { "Alice", "Beppe", "Charlie" };

    std::cout << "Set contents:\n";
    for (const auto& name : names) std::cout << name << ' ';
    std::cout << "\n";

    std::cout << "\nLookup by const char* (temporary std::string will be allocated):\n";
    auto it = names.find("Beppe"); // triggers allocation of temporary std::string
    if (it != names.end())
        std::cout << *it << " found\n";
    else
        std::cout << "Not found\n";
}

// -----------------------------
// Demo: Transparent lookup with std::string
// -----------------------------
void demo_string_transparent_lookup() {
    clear_screen();
    std::cout << "=== demo_string_transparent_lookup ===\n";

    struct TransparentLess {
        using is_transparent = void;
        bool operator()(const std::string& lhs, const std::string& rhs) const { return lhs < rhs; }
        bool operator()(const std::string& lhs, const char* rhs) const { return lhs < rhs; }
        bool operator()(const char* lhs, const std::string& rhs) const { return lhs < rhs; }
    };

    std::set<std::string, TransparentLess> names = { "Alice", "Beppe", "Charlie" };

    std::cout << "Set contents:\n";
    for (const auto& name : names) std::cout << name << ' ';
    std::cout << "\n";

    std::cout << "\nLookup by const char* (no temporary std::string should be allocated):\n";
    auto it = names.find("Beppe");
    if (it != names.end())
        std::cout << *it << " found\n";
    else
        std::cout << "Not found\n";
}

// -----------------------------
// Demo: Transparent lookup with complex object
// -----------------------------
void demo_complex_transparent_lookup() {
    clear_screen();
    std::cout << "=== demo_complex_transparent_lookup ===\n";

    struct LargeData { char buffer[1024]; }; // 1kB data

    struct CGeomObject {
        int id;
        LargeData* data;
    };

    struct CompareById {
        using is_transparent = void;

        bool operator()(const CGeomObject& lhs, const CGeomObject& rhs) const { return lhs.id < rhs.id; }
        bool operator()(const CGeomObject& lhs, int rhs) const { return lhs.id < rhs; }
        bool operator()(int lhs, const CGeomObject& rhs) const { return lhs < rhs.id; }
    };

    std::set<CGeomObject, CompareById> geomSet;

    LargeData d1, d2, d3;
    geomSet.insert({ 1, &d1 });
    geomSet.insert({ 3, &d2 });
    geomSet.insert({ 2, &d3 });

    int searchId = 2;
    auto it = geomSet.find(searchId);
    if (it != geomSet.end())
        std::cout << "Found object with id = " << it->id << '\n';
    else
        std::cout << "Object not found\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
    std::cout << "=== std::set transparent lookup demo ===\n";

    std::cout << "\n--- Base lookup (non-transparent) ---\n";
    demo_base_lookup();

    std::cout << "\n--- Transparent lookup (std::string) ---\n";
    demo_string_transparent_lookup();

    std::cout << "\n--- Transparent lookup (complex object) ---\n";
    demo_complex_transparent_lookup();

    std::cout << "\n=== End of demo ===\n";
    return 0;
}
