// map_demo.cpp
// C++17 - std::map exploration with transparent lookup and structured bindings
//
// Compile: g++ -std=c++17 map_demo.cpp -O2 -o map_demo
// MSVC: cl /std:c++17 map_demo.cpp

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <chrono>

static bool TRACE_ALLOCATIONS = true;

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
// Demo functions
// -----------------------------

void demo_basic_map() {
	std::cout << "\n=== demo_basic_map ===\n";
	std::map<std::string, int> ageMap;
	ageMap["Eve"] = 35;
	ageMap["Alice"] = 30;
	ageMap["Bob"] = 25;

	for (auto const& [name, age] : ageMap)
		std::cout << name << ": " << age << '\n';
}

void demo_custom_comparator() {
	std::cout << "\n=== demo_custom_comparator (descending) ===\n";

	struct ReverseStringCompare {
		bool operator()(const std::string& a, const std::string& b) const { return a > b; }
	};

	std::map<std::string, int, ReverseStringCompare> ageMapDesc;
	ageMapDesc["Alice"] = 30;
	ageMapDesc["Bob"] = 25;
	ageMapDesc["Charlie"] = 28;

	for (auto const& [name, age] : ageMapDesc)
		std::cout << name << ": " << age << '\n';
}

// Transparent lookup comparator
struct TransparentLess {
	using is_transparent = void;
	template<typename T1, typename T2>
	bool operator()(const T1& lhs, const T2& rhs) const { return lhs < rhs; }
};

void demo_transparent_lookup() {
	std::cout << "\n=== demo_transparent_lookup ===\n";

	std::map<std::string, int, TransparentLess> ageMap;
	ageMap["Alice"] = 30;
	ageMap["Bob"] = 25;
	ageMap["Charlie"] = 28;

	const char* key = "Bob";
	auto it = ageMap.find(key); // no temporary string allocation
	if (it != ageMap.end())
		std::cout << it->second << " found via transparent lookup\n";

	// Standard map would require std::string temp:
	std::map<std::string, int> normalMap;
	normalMap["Alice"] = 30;
	normalMap["Bob"] = 25;
	normalMap["Charlie"] = 28;

	std::string tempKey = key;
	auto it2 = normalMap.find(tempKey); // temporary string
	if (it2 != normalMap.end())
		std::cout << it2->second << " found via normal map (temporary allocation)\n";
}

// Performance demo with large number of insertions/lookups
void demo_performance() {
	std::cout << "\n=== demo_performance ===\n";

	const int N = 50000;
	std::map<std::string, int, TransparentLess> tmap;
	//std::map<std::string, int> tmap;

	for (int i = 0; i < N; ++i) tmap["Item" + std::to_string(i)] = i;

	auto start = std::chrono::high_resolution_clock::now();
	int sum = 0;
	for (int i = 0; i < N; ++i) {
		auto it = tmap.find(("Item" + std::to_string(i)).c_str());
		if (it != tmap.end()) sum += it->second;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = end - start;
	std::cout << "Sum = " << sum << ", transparent lookup took " << dur.count() << " s\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
	std::cout << "=== std::map deep dive demo ===\n";

	demo_basic_map();
	demo_custom_comparator();
	demo_transparent_lookup();
	// disable for performance demo
	TRACE_ALLOCATIONS = false;
	demo_performance();
	TRACE_ALLOCATIONS = true;

	std::cout << "\n=== End of demo ===\n";
	return 0;
}
