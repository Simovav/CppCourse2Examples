// vector_deep_demo.cpp
// C++17 - Deep std::vector exploration with global new/delete tracing and timing
//
// Compile with: g++ -std=c++17 vector_deep_demo.cpp -O2 -o vector_demo
// or with MSVC: cl /std:c++17 vector_deep_demo.cpp
//
// Now includes:
// - clear screen between demos
// - amortized cost measurement of push_back / insert
// - explicit insert demo for different positions

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>
#include <algorithm>
#include <chrono>

// -----------------------------
// Cross-platform clear screen
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
// Helper types
// -----------------------------
struct Trace {
	int value;
	Trace(int v = 0) : value(v) {
		std::cout << "  Trace::Trace(" << value << ") constructed at " << this << "\n";
	}
	Trace(const Trace& other) : value(other.value) {
		std::cout << "  Trace(copy) from " << &other << " to " << this << "\n";
	}
	Trace(Trace&& other) noexcept : value(other.value) {
		std::cout << "  Trace(move) from " << &other << " to " << this << "\n";
		other.value = -1;
	}
	Trace& operator=(const Trace& other) {
		value = other.value;
		std::cout << "  Trace::operator=(copy) to " << this << "\n";
		return *this;
	}
	Trace& operator=(Trace&& other) noexcept {
		value = other.value;
		other.value = -1;
		std::cout << "  Trace::operator=(move) to " << this << "\n";
		return *this;
	}
	~Trace() {
		std::cout << "  Trace::~Trace(" << value << ") destroyed at " << this << "\n";
	}
};

struct Point {
	int x, y;
	Point(int a = 0, int b = 0) : x(a), y(b) {
		std::cout << "  Point(" << x << "," << y << ") constructed at " << this << "\n";
	}
	~Point() {
		std::cout << "  Point(" << x << "," << y << ") destroyed at " << this << "\n";
	}
};

// -----------------------------
// Utility: print vector meta
// -----------------------------
template <typename T, typename StringLike>
void print_meta(const std::vector<T>& v, StringLike&& name) {
	std::cout << name << " -> size(): " << v.size()
		<< ", capacity(): " << v.capacity()
		<< ", data ptr: " << static_cast<const void*>(v.data()) << "\n";
}

// -----------------------------
// Demos
// -----------------------------
void demo_declaration_and_access();
void demo_push_vs_emplace();
void demo_emplace_forwarding();
void demo_capacity_and_reallocations();
void demo_clear_and_shrink_to_fit();
void demo_insert_erase_and_iterator_invalidation();
void demo_resize();
void demo_copy_and_move();
void demo_reserve_misuse();
void demo_iterator_invalidations_with_references();
void demo_data_pointer_and_moves();
void demo_insert_positions_and_timing();

// -----------------------------
// MAIN
// -----------------------------
int main() {
	std::cout << "=== std::vector deep dive demo (enhanced) ===\n";

	demo_declaration_and_access(); clear_screen();
	demo_push_vs_emplace(); clear_screen();
	demo_emplace_forwarding(); clear_screen();
	demo_capacity_and_reallocations(); clear_screen();
	demo_clear_and_shrink_to_fit(); clear_screen();
	demo_insert_erase_and_iterator_invalidation(); clear_screen();
	demo_resize(); clear_screen();
	demo_copy_and_move(); clear_screen();
	demo_reserve_misuse(); clear_screen();
	demo_iterator_invalidations_with_references(); clear_screen();
	demo_data_pointer_and_moves(); clear_screen();
	demo_insert_positions_and_timing(); clear_screen();

	std::cout << "\n=== End of demo ===\n";
	return 0;
}

// -----------------------------
// Implementations
// -----------------------------

void demo_declaration_and_access() {
	std::cout << "\n=== demo_declaration_and_access ===\n";
	std::vector<int> v1;
	std::vector<int> v2(5);
	std::vector<int> v3(5, 42);
	std::vector<int> v4 = { 1, 2, 3 };

	print_meta(v1, "v1");
	print_meta(v2, "v2");
	print_meta(v3, "v3");
	print_meta(v4, "v4");

	std::cout << "v4.front() = " << v4.front() << ", v4.back() = " << v4.back() << "\n";
}

void demo_push_vs_emplace() {
	std::cout << "\n=== demo_push_vs_emplace ===\n";
	std::vector<Trace> vec;
	print_meta(vec, "vec (initial)");

	vec.push_back(Trace(1));
	std::cout<<"\n\n";
	vec.emplace_back(2);
	std::cout << "\n\n";
	Trace t(3);
	vec.push_back(t);
	std::cout << "\n\n";
	vec.push_back(Trace(4));
	std::cout << "\n\n";

	std::cout << "Final values: ";
	for (auto& e : vec) std::cout << e.value << " ";
	std::cout << "\n";
}

void demo_capacity_and_reallocations() {
	std::cout << "\n=== demo_capacity_and_reallocations ===\n";
	std::vector<int> v;
	const void* prev_ptr = v.data();
	for (int i = 1; i <= 16; ++i) {
		v.push_back(i);
		const void* cur = v.data();
		if (cur != prev_ptr) {
			std::cout << "Reallocation at size " << v.size() << "\n";
			prev_ptr = cur;
		}
	}
}

void demo_clear_and_shrink_to_fit() {
	std::cout << "\n=== demo_clear_and_shrink_to_fit ===\n";
	std::vector<int> v;
	v.reserve(32);
	for (int i = 0; i < 20; ++i) v.push_back(i + 1);
	v.clear();
	v.push_back(100);
	v.push_back(200);
	v.shrink_to_fit();
	std::cout << "\n\n";
	std::vector<Trace> vTrace;
	vTrace.reserve(10);
	vTrace.emplace_back(0);
	vTrace.emplace_back(1);
	vTrace.emplace_back(2);
	vTrace.shrink_to_fit();
	vTrace.reserve(200);
	std::cout << "\n\n";
	vTrace = std::vector<Trace>();
}

void demo_insert_erase_and_iterator_invalidation() {
	std::cout << "\n=== demo_insert_erase_and_iterator_invalidation ===\n";
	std::vector<int> v = { 1, 2, 3, 4, 5 };
	auto it = v.begin() + 2;
	v.insert(it, 99);
	v.erase(v.begin() + 3);
}

void demo_resize() {
	std::cout << "\n=== demo_resize ===\n";
	std::vector<int> v = { 1, 2 };
	v.resize(5);
	v.resize(1);
}

void demo_copy_and_move() {
	std::cout << "\n=== demo_copy_and_move ===\n";
	std::vector<Trace> src;
	src.emplace_back(10);
	src.emplace_back(20);
	std::vector<Trace> copy = src;
	std::vector<Trace> moved = std::move(src);
}

void demo_reserve_misuse() {
	std::cout << "\n=== demo_reserve_misuse ===\n";
	std::vector<int> v;
	for (int i = 0; i < 5; ++i) {
		v.reserve(v.size() + 1);
		v.push_back(i);
	}
}

void demo_iterator_invalidations_with_references() {
	std::cout << "\n=== demo_iterator_invalidations_with_references ===\n";
	std::vector<std::string> names = { "Alice", "Bob" };
	const char* ptr = names[0].c_str();
	names.push_back("Charlie");
	std::cout << "Old c_str() ptr now possibly invalid: " << static_cast<const void*>(ptr) << "\n";
}

void demo_data_pointer_and_moves() {
	std::cout << "\n=== demo_data_pointer_and_moves ===\n";
	std::vector<Point> pts;
	pts.reserve(2);
	pts.emplace_back(1, 1);
	pts.emplace_back(2, 2);
	const void* before = pts.data();
	pts.emplace_back(3, 3);
	const void* after = pts.data();
	if (before != after)
		std::cout << "Reallocation occurred, old addresses invalid.\n";
}

// -----------------------------
// NEW: insert and timing demo
// -----------------------------
void demo_insert_positions_and_timing() {
	std::cout << "\n=== demo_insert_positions_and_timing ===\n";

	using clock = std::chrono::high_resolution_clock;

	const int N = 100000;
	std::vector<int> v1;
	auto start = clock::now();
	for (int i = 0; i < N; ++i) v1.push_back(i);
	auto end = clock::now();
	std::cout << "push_back " << N << " ints took "
		<< std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

	std::vector<int> v2;
	v2.reserve(N);
	start = clock::now();
	for (int i = 0; i < N; ++i) v2.push_back(i);
	end = clock::now();
	std::cout << "push_back with reserve(" << N << ") took "
		<< std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

	// Demonstrate insert at front (O(n))
	std::vector<int> v3;
	start = clock::now();
	for (int i = 0; i < 5000; ++i) v3.insert(v3.begin(), i);
	end = clock::now();
	std::cout << "insert at front (5000 times) took "
		<< std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

	// insert in the middle
	std::vector<int> v4;
	start = clock::now();
	for (int i = 0; i < 5000; ++i) v4.insert(v4.begin() + v4.size() / 2, i);
	end = clock::now();
	std::cout << "insert in middle (5000 times) took "
		<< std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

	std::cout << "\nObservation:\n";
	std::cout << " - push_back is amortized O(1)\n";
	std::cout << " - insert at front/middle is O(n)\n";
	std::cout << " - reserve() avoids multiple reallocations for push_back.\n";
}


// -----------------------------
// Demonstrate emplace_back forwarding and move constructor
// -----------------------------
void demo_emplace_forwarding() {
	std::cout << "\n=== demo_emplace_forwarding ===\n";

	struct MyObj {
		int x;
		std::string name;

		MyObj(int a, std::string s) : x(a), name(std::move(s)) {
			std::cout << "  MyObj(int, string) constructed at " << this
				<< " with x=" << x << ", name=" << name << "\n";
		}

		MyObj(const MyObj& other) : x(other.x), name(other.name) {
			std::cout << "  MyObj(copy) from " << &other << " to " << this << "\n";
		}

		MyObj(MyObj&& other) noexcept : x(other.x), name(std::move(other.name)) {
			std::cout << "  MyObj(move) from " << &other << " to " << this << "\n";
		}

		~MyObj() {
			std::cout << "  MyObj destroyed at " << this << " name=" << name << "\n";
		}
	};

	std::vector<MyObj> vec;

	std::string tempName = "Alice";

	std::cout << "\n-- push_back with temporary MyObj(1, \"Bob\") --\n";
	vec.push_back(MyObj(1, "Bob")); // temporary -> move
	std::cout << "\n-- emplace_back forwarding 2, tempName --\n";
	vec.emplace_back(2, tempName); // constructs in-place, no extra move/copy

	std::cout << "\n-- emplace_back with std::move(tempName) --\n";
	vec.emplace_back(3, std::move(tempName)); // moves string in-place

	std::cout << "\nFinal vector contents:\n";
	for (auto& obj : vec) {
		std::cout << "  MyObj x=" << obj.x << ", name=" << obj.name << "\n";
	}
}
