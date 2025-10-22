// set_deep_demo.cpp
// C++17 - std::set exploration with optional global new/delete tracing

#include <iostream>
#include <set>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>

// -----------------------------
// Clear screen (Windows)
// -----------------------------
void clear_screen() {
	std::system("cls");
}

// -----------------------------
// Global new/delete overrides
// -----------------------------
static bool TRACE_ALLOCATIONS = false;

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
// Helper to print set meta
// -----------------------------
template <typename Set>
void print_set(Set&& s, const std::string& name) {
	std::cout << name << " (size=" << s.size() << "): ";
	for (const auto& x : s) std::cout << x << ' ';
	std::cout << "\n";
}

// -----------------------------
// Demonstrations
// -----------------------------

// 1) Basic declaration and access
void demo_declaration_and_access() {
	clear_screen();
	std::cout << "=== demo_declaration_and_access ===\n";

	std::set<int> s1 = { 5, 1, 3, 2, 4 };
	print_set(s1, "s1");

	auto it = s1.find(3);
	if (it != s1.end())
		std::cout << "Found element: " << *it << "\n";
	else
		std::cout << "Element 3 not found\n";

	std::cout << "Count of 2: " << s1.count(2) << "\n";
}

// 2) Insert, erase, iteration
void demo_insert_erase() {
	clear_screen();
	std::cout << "=== demo_insert_erase ===\n";

	std::set<int> s;
	s.insert(10);
	s.insert(5);
	s.insert(15);
	s.insert(10); // duplicate ignored

	print_set(s, "s after inserts");

	std::cout << "\n-- erase 5 --\n";
	s.erase(5);
	print_set(s, "s after erase");

	std::cout << "\nIterate in reverse:\n";
	for (auto rit = s.rbegin(); rit != s.rend(); ++rit)
		std::cout << *rit << ' ';
	std::cout << "\n";
}

// 3) Custom comparison functor
void demo_custom_comparator() {
	clear_screen();
	std::cout << "=== demo_custom_comparator ===\n";

	struct ReverseCompare {
		bool operator()(int a, int b) const { return a > b; }
	};

	std::set<int, ReverseCompare> s;
	s.insert(1); s.insert(3); s.insert(2);

	print_set(s, "s with ReverseCompare");
}

// 4) Complex type with comparator
void demo_complex_type() {
	clear_screen();
	std::cout << "=== demo_complex_type ===\n";

	struct Point {
		int x, y;
	};

	struct ComparePoints {
		bool operator()(const Point& a, const Point& b) const {
			if (a.x != b.x) return a.x < b.x;
			return a.y < b.y;
		}
	};

	std::set<Point, ComparePoints> points;
	points.insert({ 1,2 });
	points.insert({ 0,5 });
	points.insert({ 1,1 });

	std::cout << "Points in sorted order:\n";
	for (const auto& p : points)
		std::cout << '(' << p.x << ',' << p.y << ") ";
	std::cout << "\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
	std::cout << "=== std::set deep dive demo ===\n";

	demo_declaration_and_access();
	demo_insert_erase();
	demo_custom_comparator();
	demo_complex_type();

	std::cout << "\n=== End of demo ===\n";
	return 0;
}
