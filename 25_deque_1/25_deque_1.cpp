// deque_deep_demo.cpp
// C++17 - Detailed std::deque exploration with global new/delete tracing
//
// Compile with: g++ -std=c++17 deque_deep_demo.cpp -O2 -o deque_demo
// or with MSVC: cl /std:c++17 deque_deep_demo.cpp

#include <iostream>
#include <deque>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>
#include <algorithm>

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
	if (n == 0) n = 1;
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
// Helper to print deque meta
// -----------------------------
template <typename T, typename StringLike>
void print_meta(const std::deque<T>& d, StringLike name) {
	std::cout << name << " -> size(): " << d.size() << ", front: "
		<< (d.empty() ? -1 : d.front()) << ", back: "
		<< (d.empty() ? -1 : d.back()) << "\n";
}

// -----------------------------
// Demonstrations
// -----------------------------

// 1) Declaration and basic access
void demo_declaration_and_access() {
	clear_screen();
	std::cout << "=== demo_declaration_and_access ===\n";

	std::deque<int> d1;
	std::deque<int> d2(5);
	std::deque<int> d3(5, 42);
	std::deque<int> d4 = { 1,2,3 };

	print_meta(d1, "d1");
	print_meta(d2, "d2");
	print_meta(d3, "d3");
	print_meta(d4, "d4");

	std::cout << "d4[0] = " << d4[0] << ", d4.at(2) = " << d4.at(2) << "\n";
	std::cout << "d4.front() = " << d4.front() << ", d4.back() = " << d4.back() << "\n";
}

// 2) push_back, push_front, emplace_back, emplace_front
void demo_push_emplace() {
	clear_screen();
	std::cout << "=== demo_push_emplace ===\n";

	std::deque<int> d;
	print_meta(d, "d initial");

	std::cout << "\n-- push_back 10 --\n";
	d.push_back(10);
	print_meta(d, "d");

	std::cout << "\n-- push_front 5 --\n";
	d.push_front(5);
	print_meta(d, "d");

	std::cout << "\n-- emplace_back 20 --\n";
	d.emplace_back(20);
	print_meta(d, "d");

	std::cout << "\n-- emplace_front 1 --\n";
	d.emplace_front(1);
	print_meta(d, "d");

	std::cout << "Contents: ";
	for (int x : d) std::cout << x << " ";
	std::cout << "\n";
}

// 3) pop_back, pop_front
void demo_pop_back_front() {
	clear_screen();
	std::cout << "=== demo_pop_back_front ===\n";

	std::deque<int> d = { 1,2,3,4 };
	print_meta(d, "d initial");

	std::cout << "\n-- pop_back --\n";
	d.pop_back();
	print_meta(d, "d");

	std::cout << "\n-- pop_front --\n";
	d.pop_front();
	print_meta(d, "d");

	std::cout << "Contents: ";
	for (int x : d) std::cout << x << " ";
	std::cout << "\n";
}

// 4) insert, erase
void demo_insert_erase() {
	clear_screen();
	std::cout << "=== demo_insert_erase ===\n";

	std::deque<int> d = { 10,20,30 };
	print_meta(d, "d initial");

	std::cout << "\n-- insert 15 at position 1 --\n";
	d.insert(d.begin() + 1, 15);
	print_meta(d, "d");
	std::cout << "Contents: ";
	for (int x : d) std::cout << x << " ";
	std::cout << "\n";

	std::cout << "\n-- erase element at position 2 --\n";
	d.erase(d.begin() + 2);
	print_meta(d, "d");
	std::cout << "Contents: ";
	for (int x : d) std::cout << x << " ";
	std::cout << "\n";
}

// 5) resize and clear
void demo_resize_clear() {
	clear_screen();
	std::cout << "=== demo_resize_clear ===\n";

	std::deque<int> d = { 1,2,3 };
	print_meta(d, "d initial");

	std::cout << "\n-- resize(5) --\n";
	d.resize(5);
	print_meta(d, "d");
	std::cout << "Contents: ";
	for (int x : d) std::cout << x << " ";
	std::cout << "\n";

	std::cout << "\n-- clear() --\n";
	d.clear();
	print_meta(d, "d after clear");
}

// 6) copy and move
void demo_copy_move() {
	clear_screen();
	std::cout << "=== demo_copy_move ===\n";

	std::deque<int> src = { 1,2,3 };
	print_meta(src, "src initial");

	std::cout << "\n-- copy construct dst = src --\n";
	std::deque<int> dst = src;
	print_meta(src, "src after copy");
	print_meta(dst, "dst");

	std::cout << "\n-- move construct d2 = std::move(src) --\n";
	std::deque<int> d2 = std::move(src);
	print_meta(src, "src after move");
	print_meta(d2, "d2 moved-from src");
}

// 7) performance comparison: push_front vs push_back large deque
#include <chrono>
void demo_performance_push() {
	clear_screen();
	std::cout << "=== demo_performance_push ===\n";

	const int N = 100000;
	std::deque<int> d_back, d_front;

	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) d_back.push_back(i);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur_back = t2 - t1;

	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) d_front.push_front(i);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur_front = t2 - t1;

	std::cout << "push_back " << N << " elems: " << dur_back.count() << " s\n";
	std::cout << "push_front " << N << " elems: " << dur_front.count() << " s\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
	std::cout << "=== std::deque deep dive demo (with global new/delete tracing) ===\n";

	demo_declaration_and_access();
	demo_push_emplace();
	demo_pop_back_front();
	demo_insert_erase();
	demo_resize_clear();
	demo_copy_move();
	TRACE_ALLOCATIONS = false; // disable tracing for performance demo
	demo_performance_push();
	TRACE_ALLOCATIONS = true; // re-enable tracing

	std::cout << "\n=== End of demo ===\n";
	return 0;
}
