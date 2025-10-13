#include <iostream>
#include <type_traits>
#include <utility>

// ===============================================================
// TYPE DISPLAYER — intentionally undefined, causes compile error
// when instantiated, revealing deduced types in the compiler output
// ===============================================================
template <typename T>
class TD;

// ===============================================================
// 0. Helper: LoudMan
// ===============================================================
// Reports if called on lvalue, const lvalue, or rvalue
class LoudMan {
public:
	void Type()& {
		std::cout << "LoudMan says: I'm an LVALUE!\n";
	}

	void Type() const& {
		std::cout << "LoudMan says: I'm a CONST LVALUE!\n";
	}

	void Type()&& {
		std::cout << "LoudMan says: I'm an RVALUE!\n";
	}
};

// ===============================================================
// 1. LVALUE REFERENCE DEDUCTION
// ===============================================================
template <typename T>
void f_lvalue(T& param) {
	TD<T> t;
	TD<decltype(param)> p;
}

void example_lvalue_refs() {
	int x = 3;
	const int cx = 3;
	int& rx = x;
	const int& rcx = cx;

	//f_lvalue(x);   // T = int,       param = int&
	//f_lvalue(cx);  // T = const int, param = const int&
	//f_lvalue(rx);  // T = int,       param = int&
	//f_lvalue(rcx); // T = const int, param = const int&
}

// ===============================================================
// 2. CONST LVALUE REFERENCES
// ===============================================================
template <typename T>
void f_const_lvalue(const T& param) {
	TD<T> t;
	TD<decltype(param)> p;
}

void example_const_lvalue_refs() {
	int x = 5;
	const int cx = 10;
	//f_const_lvalue(x);
	//f_const_lvalue(cx);
}

// ===============================================================
// 3. BY VALUE
// ===============================================================
template <typename T>
void f_by_value(T param) {
	TD<T> t;
	TD<decltype(param)> p;
}

void example_by_value() {
	int x = 3;
	const int cx = 3;
	int& rx = x;

	//f_by_value(x);
	//f_by_value(cx);
	//f_by_value(rx);
}

// ===============================================================
// 4. POINTERS
// ===============================================================
template <typename T>
void f_pointer(T* ptr) {
	TD<T> t;
	TD<decltype(ptr)> p;
}

void example_pointers() {
	int x = 42;
	int* px = &x;

	//f_pointer(px);
	//f_pointer(&x);
}

// ===============================================================
// 5. ARRAYS
// ===============================================================
template <typename T>
void f_array(T arr[]) {
	TD<T> t;
	TD<decltype(arr)> p;
}

void example_arrays() {
	int arr[5];
	//f_array(arr); // T deduced as int, param = int*
}

// ===============================================================
// 6. FUNCTION POINTERS
// ===============================================================
template <typename T>
void f_funcptr(T func) {
	// TD<T> t;
	// TD<decltype(func)> p;
}

void g(int) {}

void example_function_pointers() {
	f_funcptr(g); // T = void(*)(int)
}

// ===============================================================
// 7. FORWARDING REFERENCES
// ===============================================================
template <typename T>
void f_forward(T&& param) {
	TD<T> t;
	TD<decltype(param)> p;
}

void example_forwarding_refs() {
	int x = 5;
	const int cx = 42;

	//f_forward(x);   // lvalue -> T = int&, param = int&
	//f_forward(10);  // rvalue -> T = int,  param = int&&
	//f_forward(cx);  // const lvalue -> T = const int&, param = const int&
}

// ===============================================================
// 8. PERFECT FORWARDING DEMONSTRATION (WITH LOUDMAN)
// ===============================================================
template <typename T>
void forward_wrapper(T&& obj) {
	std::cout << "Inside wrapper -> calling Type() after std::forward:\n";
	std::forward<T>(obj).Type();
}

void example_perfect_forwarding() {
	LoudMan lm;
	const LoudMan clm;

	//T = LoudMan& -- param = LoudMan&
	std::cout << "\n-- Passing lvalue LoudMan --\n";
	forward_wrapper(lm);

	//T = const LoudMan& -- param = const LoudMan&
	std::cout << "\n-- Passing const lvalue LoudMan --\n";
	forward_wrapper(clm);

	//T = LoudMan -- param = LoudMan&&
	std::cout << "\n-- Passing temporary LoudMan --\n";
	forward_wrapper(LoudMan{});
}

// ===============================================================
// 9. FORWARDING TRAP (T&& MEMBER IS NOT FORWARDING REF)
// ===============================================================
template <typename T>
class Box_Bad {
	T value;
public:
	Box_Bad(T&& v) : value(std::forward<T>(v)) {}
};

void example_forwarding_trap() {
	Box_Bad b1(10);
	int x = 10;
	//Box_Bad b2(x); // ❌ Uncomment to see compilation error
	//Box_Bad<int> b2(x); // ❌ Uncomment to see compilation error
}

// ===============================================================
// 10. CORRECT FORWARDING CONSTRUCTOR (WITH LOUDMAN)
// ===============================================================
template <typename T>
class Box_Good {
	T value;
public:
	template <typename U>
	Box_Good(U&& v) : value(std::forward<U>(v)) {
		std::cout << "Box_Good constructed with U = ";
		if constexpr (std::is_lvalue_reference_v<U>)
			std::cout << "lvalue reference\n";
		else if constexpr (std::is_rvalue_reference_v<U&&>)
			std::cout << "rvalue reference\n";
		else
			std::cout << "value (no ref)\n";

		// Let’s see what’s inside:
		//value.Type(); wrong, always lvalue or const lvalue
		std::forward<U>(value).Type();
	}
};

void example_forwarding_constructor() {
	std::cout << "\n=== Box_Good Forwarding Constructor ===\n";

	LoudMan lm;
	const LoudMan clm;

	std::cout << "\n-- Constructing with lvalue LoudMan --\n";
	Box_Good<LoudMan> b1(lm);

	std::cout << "\n-- Constructing with const lvalue LoudMan --\n";
	Box_Good<LoudMan> b2(clm);

	std::cout << "\n-- Constructing with temporary LoudMan --\n";
	Box_Good<LoudMan> b3(LoudMan{});
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
	std::cout << "=== TEMPLATE TYPE DEDUCTION DEMOS ===\n";
	//example_lvalue_refs();
	//example_const_lvalue_refs();
	//example_by_value();
	//example_pointers();
	//example_arrays();
	//example_function_pointers();
	//example_forwarding_refs();
	//example_perfect_forwarding();
	//example_forwarding_trap();
	//example_forwarding_constructor();
}
