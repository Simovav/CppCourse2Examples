#include <iostream>
#include <utility> // per std::forward

// ===============================================================
// 1. Variadic Templates: recursive expansion
// ===============================================================
void printAll() {
	std::cout << "\n"; // base case: termina la ricorsione
}

template <typename T, typename... Rest>
void printAll(T first, Rest... rest) {
	std::cout << first << ' ';
	printAll(rest...); // espande ricorsivamente il pack
}

template <typename... Args>
void forwardToPrint(Args... args) {
	printAll(args...); // forwarding semplice (copia i valori)
}

// ===============================================================
// 2. Variadic Templates + perfect forwarding
// ===============================================================
void printAllForwarded() { std::cout << "\n"; }

template <typename T, typename... Rest>
void printAllForwarded(T&& first, Rest&&... rest) {
	std::cout << first << ' ';
	printAllForwarded(std::forward<Rest>(rest)...); // forwarding efficiente
}

template <typename... Args>
void forwardToPrintPerfect(Args&&... args) {
	printAllForwarded(std::forward<Args>(args)...);
}

// ===============================================================
// 3. Fold expression examples (C++17)
// ===============================================================
template <typename... Args>
auto sum(Args... args) {
	return (0 + ... + args); // left fold: 0 + arg1 + arg2 + ...
}

template <typename... Args>
void printAllFold(Args&&... args) {
	(std::cout << ... << args) << '\n'; // fold expression per stampare tutti
}

template <typename... Args>
bool allTrue(Args... args) {
	return (... && args); // fold expression con AND logico
}

template <typename... Args>
bool anyTrue(Args... args) {
	return (args || ...); // fold expression con OR logico
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
	std::cout << "=== Recursive variadic template ===\n";
	forwardToPrint(1, 2.5, "hello"); // prints: 1 2.5 hello

	std::cout << "\n=== Perfect forwarding variadic template ===\n";
	int x = 10;
	forwardToPrintPerfect(x, 2.5, "world"); // prints: 10 2.5 world

	std::cout << "\n=== Fold expression: sum ===\n";
	std::cout << "sum(1,2,3,4) = " << sum(1, 2, 3, 4) << '\n'; // prints 10

	std::cout << "\n=== Fold expression: print all ===\n";
	printAllFold(1, " + ", 2, " = ", 3); // prints: 1 + 2 = 3

	std::cout << "\n=== Fold expression: logical operators ===\n";
	std::cout << std::boolalpha;
	std::cout << "allTrue(true, true, false) = " << allTrue(true, true, false) << '\n'; // false
	std::cout << "anyTrue(false, false, true) = " << anyTrue(false, false, true) << '\n'; // true

	return 0;
}
