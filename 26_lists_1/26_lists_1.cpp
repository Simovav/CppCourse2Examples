// list_deep_demo.cpp
// C++17 - Detailed std::list and std::forward_list exploration
//
// Compile with: g++ -std=c++17 list_deep_demo.cpp -O2 -o list_demo
// or with MSVC: cl /std:c++17 list_deep_demo.cpp

#include <iostream>
#include <list>
#include <forward_list>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>
#include <chrono>

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
// Helper to print list meta
// -----------------------------
template <typename T, typename StringLike>
void print_meta(const std::list<T>& l, StringLike name) {
    std::cout << name << " -> size(): " << l.size()
        << ", front: " << (l.empty() ? -1 : l.front())
        << ", back: " << (l.empty() ? -1 : l.back()) << "\n";
}

template <typename T, typename StringLike>
void print_meta(const std::forward_list<T>& l, StringLike name) {
    size_t sz = 0; T back{};
    for (auto& x : l) { back = x; ++sz; }
    std::cout << name << " -> size(): " << sz
        << ", front: " << (sz == 0 ? -1 : l.front())
        << ", back: " << (sz == 0 ? -1 : back) << "\n";
}

// -----------------------------
// Demonstrations
// -----------------------------

void demo_list_basic() {
    clear_screen();
    std::cout << "=== demo_list_basic ===\n";

    std::list<int> l1;
    std::list<int> l2(5);
    std::list<int> l3(5, 42);
    std::list<int> l4 = { 1,2,3 };

    print_meta(l1, "l1");
    print_meta(l2, "l2");
    print_meta(l3, "l3");
    print_meta(l4, "l4");

    std::cout << "l4.front() = " << l4.front() << ", l4.back() = " << l4.back() << "\n";
}

void demo_list_push_pop() {
    clear_screen();
    std::cout << "=== demo_list_push_pop ===\n";

    std::list<int> l;

    l.push_back(10);
    l.push_front(5);
    l.emplace_back(20);
    l.emplace_front(1);

    print_meta(l, "l after pushes/emplace");
    std::cout << "Contents: ";
    for (auto& x : l) std::cout << x << " ";
    std::cout << "\n";

    l.pop_back();
    l.pop_front();
    print_meta(l, "l after pops");
}

void demo_list_insert_erase() {
    clear_screen();
    std::cout << "=== demo_list_insert_erase ===\n";

    std::list<int> l = { 10,20,30 };
    auto it = l.begin(); ++it;
    l.insert(it, 15); // before second element
    print_meta(l, "after insert 15");
    std::cout << "Contents: ";
    for (auto& x : l) std::cout << x << " ";
    std::cout << "\n";

    it = l.begin(); ++it; // second element
    l.erase(it);
    print_meta(l, "after erase second element");
    std::cout << "Contents: ";
    for (auto& x : l) std::cout << x << " ";
    std::cout << "\n";
}

void demo_list_splice() {
    clear_screen();
    std::cout << "=== demo_list_splice ===\n";

    std::list<int> l1 = { 1,2,3 };
    std::list<int> l2 = { 10,20,30 };

    auto it = l1.begin(); ++it; // position 1
    l1.splice(it, l2); // move all l2 into l1 before it

    std::cout << "After splice:\nContents of l1: ";
    for (auto& x : l1) std::cout << x << " ";
    std::cout << "\nContents of l2 (should be empty): " << l2.size() << "\n";
}

void demo_list_trace() {
    clear_screen();
    std::cout << "=== demo_list_trace (push vs emplace with Trace) ===\n";

    std::list<Trace> l;
    std::cout << "-- push_back(Trace(1)) --\n";
    l.push_back(Trace(1));

    std::cout << "-- emplace_back(2) --\n";
    l.emplace_back(2);

    std::cout << "-- push_back with named Trace t(3) --\n";
    Trace t(3);
    l.push_back(t);

    std::cout << "-- push_back with move Trace(4) --\n";
    l.push_back(Trace(4));

    std::cout << "Final contents values: ";
    for (auto& e : l) std::cout << e.value << " ";
    std::cout << "\n";
}

void demo_forward_list() {
    clear_screen();
    std::cout << "=== demo_forward_list ===\n";

    std::forward_list<int> fl = { 1,2,3 };
    fl.push_front(0);
    fl.emplace_front(-1);

    print_meta(fl, "fl");
    std::cout << "Contents: ";
    for (auto& x : fl) std::cout << x << " ";
    std::cout << "\n";

    fl.pop_front();
    print_meta(fl, "fl after pop_front");
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
    std::cout << "=== std::list / std::forward_list deep dive demo ===\n";

    demo_list_basic();
    demo_list_push_pop();
    demo_list_insert_erase();
    demo_list_splice();
    demo_list_trace();
    demo_forward_list();

    std::cout << "\n=== End of demo ===\n";
    return 0;
}
