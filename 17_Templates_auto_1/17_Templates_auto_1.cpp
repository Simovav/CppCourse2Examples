#include <iostream>
#include <string>
#include <vector>

auto add(int a, int b) {
	return a + b;
}

int main() {
	std::cout << "=== Type Deduction with auto ===\n";

	// --- Basic auto ---
	int x = 42;
	auto a = x;        // int
	const auto b = x;  // const int
	auto& c = x;       // int&
	auto* p = &x;      // int*

	std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", *p = " << *p << "\n";

	// --- Rules of auto deduction ---
	const int cx = 100;
	int& rx = x;

	auto a1 = x;   // int
	auto a2 = cx;  // int (const stripped)
	auto a3 = rx;  // int (reference stripped)
	auto& a4 = rx; // int& (reference preserved)
	auto&& a5 = x; // int&& becomes int& here (forwarding reference)

	std::cout << "a1=" << a1 << ", a2=" << a2 << ", a3=" << a3 << ", a4=" << a4 << ", a5=" << a5 << "\n";

	// --- Using auto with functions ---
	auto result = add(2, 3); // deduced as int
	std::cout << "add(2,3) = " << result << "\n";

	// --- Auto and lambdas ---
	auto lambda = [](auto&& param) { return param + 1; };
	int y = 5;

	std::cout << "lambda(y) = " << lambda(y) << "\n";   // param deduced as int& (lvalue)
	std::cout << "lambda(10) = " << lambda(10) << "\n"; // param deduced as int (rvalue)

	// --- Auto with STL containers ---
	std::vector<int> vec = { 1,2,3,4,5 };
	for (auto v : vec) {          // auto deduced as int
		std::cout << v << " ";
	}
	std::cout << "\n";

	for (auto& v : vec) {         // auto deduced as int&
		v *= 2;                   // modifies original vector
	}

	for (const auto& v : vec) {   // auto deduced as const int&
		std::cout << v << " ";
	}
	std::cout << "\n";

	return 0;
}
