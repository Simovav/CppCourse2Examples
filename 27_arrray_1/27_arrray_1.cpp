// array_deep_demo.cpp
// C++17 - Detailed std::array exploration
//
// Compile with: g++ -std=c++17 array_deep_demo.cpp -O2 -o array_demo
// or with MSVC: cl /std:c++17 array_deep_demo.cpp

#include <iostream>
#include <array>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>

static bool TRACE_ALLOCATIONS = true;

// -----------------------------
// Clear screen (Windows)
// -----------------------------
void clear_screen() {
    std::system("cls");
}

// -----------------------------
// Global new/delete overrides
// -----------------------------
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
// Helper class to trace moves/copies
// -----------------------------
struct Trace {
    int value;
    Trace(int v = 0) : value(v) { std::cout << "Trace(" << value << ") constructed at " << this << "\n"; }
    Trace(const Trace& other) : value(other.value) { std::cout << "Trace copy " << value << " from " << &other << " to " << this << "\n"; }
    Trace(Trace&& other) noexcept : value(other.value) { std::cout << "Trace move " << value << " from " << &other << " to " << this << "\n"; other.value = -1; }
    Trace& operator=(const Trace& other) { value = other.value; std::cout << "Trace assign copy " << value << " to " << this << "\n"; return *this; }
    Trace& operator=(Trace&& other) noexcept { value = other.value; other.value = -1; std::cout << "Trace assign move " << value << " to " << this << "\n"; return *this; }
    ~Trace() { std::cout << "Trace(" << value << ") destroyed at " << this << "\n"; }
};

// -----------------------------
// Demonstrations
// -----------------------------
void demo_array_basic() {
    clear_screen();
    std::cout << "=== demo_array_basic ===\n";

    std::array<int, 5> arr1 = { 1,2,3,4,5 };
    std::array<int, 5> arr2; // default-initialized (zero-initialized for ints if global/static)

    std::cout << "arr1 contents: ";
    for (auto& x : arr1) std::cout << x << " ";
    std::cout << "\n";

    arr1[0] = 10;
    std::cout << "arr1 after arr1[0] = 10: ";
    for (auto x : arr1) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Access arr1.at(4): " << arr1.at(4) << "\n";
    std::cout << "Front: " << arr1.front() << ", Back: " << arr1.back() << "\n";

    int* p = arr1.data();
    std::cout << "Pointer to underlying data: " << p << "\n";
}

void demo_array_iterators() {
    clear_screen();
    std::cout << "=== demo_array_iterators ===\n";

    std::array<int, 5> arr = { 1,2,3,4,5 };
    std::cout << "Forward iteration: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) std::cout << *it << " ";
    std::cout << "\n";

    std::cout << "Reverse iteration: ";
    for (auto rit = arr.rbegin(); rit != arr.rend(); ++rit) std::cout << *rit << " ";
    std::cout << "\n";
}

void demo_array_fill_swap() {
    clear_screen();
    std::cout << "=== demo_array_fill_swap ===\n";

    std::array<int, 5> arr1 = { 1,2,3,4,5 };
    std::array<int, 5> arr2 = { 10,20,30,40,50 };

    std::cout << "Before fill and swap:\narr1: ";
    for (auto x : arr1) std::cout << x << " ";
    std::cout << "\narr2: ";
    for (auto x : arr2) std::cout << x << " ";
    std::cout << "\n";

    arr1.fill(0);
    arr1.swap(arr2);

    std::cout << "After fill and swap:\narr1: ";
    for (auto x : arr1) std::cout << x << " ";
    std::cout << "\narr2: ";
    for (auto x : arr2) std::cout << x << " ";
    std::cout << "\n";
}

void demo_array_trace() {
    clear_screen();
    std::cout << "=== demo_array_trace (copy/move of Trace) ===\n";

    std::array<Trace, 3> arr = { Trace(1), Trace(2), Trace(3) };
    std::cout << "Initial array contents values: ";
    for (auto& t : arr) std::cout << t.value << " ";
    std::cout << "\n";

    std::cout << "-- Copy construct arr2 = arr --\n";
    std::array<Trace, 3> arr2 = arr;

    std::cout << "-- Move construct arr3 = std::move(arr) --\n";
    std::array<Trace, 3> arr3 = std::move(arr);

    std::cout << "Final arr contents: ";
    for (auto& t : arr) std::cout << t.value << " ";
    std::cout << "\n";

    std::cout << "Final arr2 contents: ";
    for (auto& t : arr2) std::cout << t.value << " ";
    std::cout << "\n";

    std::cout << "Final arr3 contents: ";
    for (auto& t : arr3) std::cout << t.value << " ";
    std::cout << "\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
    std::cout << "=== std::array deep dive demo ===\n";

    demo_array_basic();
    demo_array_iterators();
    demo_array_fill_swap();
    demo_array_trace();

    std::cout << "\n=== End of demo ===\n";
    return 0;
}
