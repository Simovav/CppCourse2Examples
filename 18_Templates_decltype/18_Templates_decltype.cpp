#include <iostream>
#include <utility> // per std::move

// --- Variabili globali per le funzioni ---
int x = 0;
const int cy = 42;

// --- Funzioni dimostrative di auto vs decltype(auto) ---
auto f1() { return (x); }           // auto: reference dropped, tipo int
decltype(auto) f2() { return x; } // decltype(auto): drop reference, copia
decltype(auto) f3() { return (x); } // decltype(auto): reference to x

auto g1() { return cy; }            // auto: const dropped, tipo int
decltype(auto) g2() { return cy; }  // decltype(auto): const preserved, tipo const int

// Per ottenere una reference con decltype(auto), bisogna scrivere return con parentesi:
decltype(auto) f_ref() { return (x); } // con parentesi: int&

int main() {
    std::cout << "=== decltype with variables ===\n";

    int a1 = 1;
    decltype(a1) a = 1;       // int
    int& rx = a1;
    decltype(rx) b = a1;      // int&

    const int cx = 2;
    decltype(cx) c = 3;       // const int

    const int& rcx = a1;
    decltype(rcx) d = a1;     // const int&

    int&& rv = 5;
    decltype(rv) e = 10;      // int&&

    std::cout << "a=" << a << ", b=" << b << ", c=" << c
        << ", d=" << d << ", e=" << e << "\n";

    std::cout << "\n=== decltype with expressions ===\n";

    int x_local = 100;

    decltype(x_local) f = 4;           // int
    decltype((x_local)) g = x_local;   // int& (lvalue expression)
    g = 20;                             // modifica x_local
    decltype(x_local + 2) h = 5;       // int (prvalue)
    decltype(std::move(x_local)) i = 30; // int&& (xvalue)
    i = 40;

    std::cout << "x_local=" << x_local << ", g=" << g
        << ", h=" << h << ", i=" << i << "\n";

    std::cout << "\n=== decltype(auto) vs auto in functions ===\n";

    std::cout << "f1()=" << f1() << " (auto, copia)"
        << ", f2()=" << f2() << " (decltype(auto), copia)"
        << ", g1()=" << g1() << " (auto, copia)"
        << ", g2()=" << g2() << " (decltype(auto), const int)\n";

    std::cout << "f_ref()=" << f_ref() << " (decltype(auto) con parentesi, reference)\n";

    std::cout << "\n=== decltype(auto) and dangling references ===\n";

    auto foo1 = []() -> int& {
        static int x_static = 10;
        return x_static; // sicuro, reference a static
        };

    auto foo2 = []() -> decltype(auto) {
        static int x_static = 20;
        return x_static; //  senza parentesi: ritorna copia
        };

    auto foo2_ref = []() -> decltype(auto) {
        static int x_static = 30;
        return (x_static); // con parentesi: ritorna reference
        };

    auto foo3 = []() -> decltype(auto) {
        int local = 5;
        return (local); //  dangling reference, NON usare
        };

    std::cout << "foo1()=" << foo1() << ", foo2()=" << foo2()
        << ", foo2_ref()=" << foo2_ref() << "\n";
    // std::cout << foo3() << "\n"; // commented: unsafe, dangling reference

    std::cout << "\n=== Auto with forwarding references ===\n";

    int y = 50;
    auto&& fwd1 = y;        // int& (lvalue forwarding reference)
    auto&& fwd2 = 10;       // int&& (rvalue forwarding reference)

    std::cout << "fwd1=" << fwd1 << ", fwd2=" << fwd2 << "\n";

    return 0;
}
