#include <iostream>
#include <type_traits>
#include <string>

// ===============================================================
// 1. STATIC_ASSERT: manual type check
// ===============================================================
template <typename T>
void PrintTwice(const T& value) {
    // Require T to be copy constructible
    static_assert(std::is_copy_constructible<T>::value,
        "T must be copy-constructible!");

    std::cout << value << " " << value << "\n";
}

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete; // ❌ not copyable
};

// ===============================================================
// 2. ENABLE_IF: restrict template overloads (SFINAE)
// ===============================================================

// Works only for integral types
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
AddOne(T value) {
    std::cout << "Integral AddOne: " << value + 1 << "\n";
}

// Works only for floating-point types
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
AddOne(T value) {
    std::cout << "Floating AddOne: " << value + 1.0 << "\n";
}

// ===============================================================
// 3. ENABLE_IF IN CLASS TEMPLATES
// ===============================================================
template <typename T, typename = typename std::enable_if<std::is_copy_constructible<T>::value>::type>
class Wrapper {
public:
    T value;
    Wrapper(const T& v) : value(v) {
        std::cout << "Wrapper created for a copyable type\n";
    }
};

// ===============================================================
// 4. ENABLE_IF + INVOCABLE
// ===============================================================

// Function that only accepts callable types
template <typename F>
typename std::enable_if<std::is_invocable<F>::value, void>::type
CallIfCallable(F&& f) {
    f();
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
    std::cout << "=== TEMPLATE RESTRICTIONS (pre-C++20) ===\n\n";

    // --- 1. static_assert example ---
    std::cout << "[1] Static Assert Example:\n";
    PrintTwice(std::string("Hello"));
    // PrintTwice(NonCopyable{}); // ❌ Uncomment to see compile-time error

    // --- 2. enable_if overloads ---
    std::cout << "\n[2] SFINAE with enable_if (function overloads):\n";
    AddOne(10);     // integral -> calls first
    AddOne(3.14);   // floating -> calls second

    // --- 3. enable_if in class templates ---
    std::cout << "\n[3] SFINAE in class templates:\n";
    Wrapper<std::string> w1("OK");
    // Wrapper<NonCopyable> w2(NonCopyable{}); // ❌ Uncomment to see compile-time error

    // --- 4. enable_if with callable ---
    std::cout << "\n[4] Callable detection:\n";
    auto lambda = []() { std::cout << "Lambda called!\n"; };
    CallIfCallable(lambda);

    // int x = 42;
    // CallIfCallable(x); // ❌ Uncomment to see compile-time error

    std::cout << "\nAll valid examples compiled and executed successfully.\n";
}
