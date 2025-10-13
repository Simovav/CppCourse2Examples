#include <iostream>
#include <concepts>
#include <string>
#include <type_traits>

// ===============================================================
// 1. CONCEPTS: manual type check
// ===============================================================
template <typename T>
concept Copyable = std::is_copy_constructible_v<T>;

template <Copyable T>
void PrintTwice(const T& value) {
    std::cout << value << " " << value << "\n";
}

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete; // ❌ not copyable
};

// ===============================================================
// 2. CONCEPTS WITH FUNCTION OVERLOADS
// ===============================================================

// Only integral types
template <std::integral T>
void AddOne(T value) {
    std::cout << "Integral AddOne: " << value + 1 << "\n";
}

// Only floating-point types
template <std::floating_point T>
void AddOne(T value) {
    std::cout << "Floating AddOne: " << value + 1.0 << "\n";
}

// ===============================================================
// 3. CONCEPTS IN CLASS TEMPLATES
// ===============================================================
template <Copyable T>
class Wrapper {
public:
    T value;
    Wrapper(const T& v) : value(v) {
        std::cout << "Wrapper created for a copyable type\n";
    }
};

// ===============================================================
// 4. CONCEPTS + INVOCABLE
// ===============================================================
template <typename F>
    requires std::invocable<F>
void CallIfCallable(F&& f) {
    f();
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
    std::cout << "=== TEMPLATE RESTRICTIONS (C++20 with concepts) ===\n\n";

    // --- 1. concept example ---
    std::cout << "[1] Concept Example:\n";
    PrintTwice(std::string("Hello"));
    // PrintTwice(NonCopyable{}); // ❌ Compile-time error

    // --- 2. concept overloads ---
    std::cout << "\n[2] Concept overloads:\n";
    AddOne(10);     // integral
    AddOne(3.14);   // floating-point

    // --- 3. class templates with concept ---
    std::cout << "\n[3] Class template with concept:\n";
    Wrapper<std::string> w1("OK");
    // Wrapper<NonCopyable> w2(NonCopyable{}); // ❌ Compile-time error

    // --- 4. callable detection ---
    std::cout << "\n[4] Callable detection:\n";
    auto lambda = []() { std::cout << "Lambda called!\n"; };
    CallIfCallable(lambda);
    // int x = 42;
    // CallIfCallable(x); // ❌ Compile-time error

    std::cout << "\nAll valid examples compiled and executed successfully.\n";
}
