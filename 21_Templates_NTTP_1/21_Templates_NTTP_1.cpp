#include <iostream>
#include <array>
#include <vector>

// ===============================================================
// 1. NTTP con integer
// ===============================================================
template <std::size_t N>
struct FixedBuffer {
    std::array<int, N> data{};
};

void demoIntegerNTTP() {
    FixedBuffer<4> buf;
    std::cout << "FixedBuffer size: " << buf.data.size() << "\n";
}

// ===============================================================
// 2. NTTP con bool
// ===============================================================
template <bool Debug>
void process() {
    if constexpr (Debug) {
        std::cout << "Debug mode enabled\n";
    }
    else {
        std::cout << "Release mode\n";
    }
}

void demoBoolNTTP() {
    process<true>();
    process<false>();
}

// ===============================================================
// 3. NTTP con enum class
// ===============================================================
enum class Mode { Debug, Release, Profile };

template <Mode M>
void run() {
    if constexpr (M == Mode::Debug) {
        std::cout << "Running in Debug mode\n";
    }
    else if constexpr (M == Mode::Release) {
        std::cout << "Running in Release mode\n";
    }
    else {
        std::cout << "Running in Profile mode\n";
    }
}

void demoEnumNTTP() {
    run<Mode::Debug>();
    run<Mode::Release>();
    run<Mode::Profile>();
}

// ===============================================================
// 4. Template default arguments per tipo
// ===============================================================
template <typename T, typename Alloc = std::allocator<T>>
class MyVector {
    std::vector<T, Alloc> v;
public:
    void add(const T& x) { v.push_back(x); }
    void print() {
        for (auto& e : v) std::cout << e << " ";
        std::cout << "\n";
    }
};

void demoDefaultTypeArg() {
    MyVector<int> mv; // usa std::allocator<int> come default
    mv.add(42); mv.add(7);
    mv.print();
}

// ===============================================================
// 5. NTTP con valore di default
// ===============================================================
template <int N = 16>
struct Buffer {
    int data[N]{};
};

void demoDefaultNTTP() {
    Buffer<> buf1;   // N = 16 (default)
    Buffer<8> buf2;  // N = 8
    std::cout << "buf1 size: " << sizeof(buf1.data) / sizeof(int) << "\n";
    std::cout << "buf2 size: " << sizeof(buf2.data) / sizeof(int) << "\n";
}

// ===============================================================
// 6. Combinazione NTTP + default type
// ===============================================================
template <typename T = int, int N = 4>
struct Matrix {
    T data[N][N]{};
    void print() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                std::cout << data[i][j] << ' ';
            std::cout << '\n';
        }
    }
};

void demoCombinedNTTP() {
    Matrix<> m1;       // int, N=4
    Matrix<double> m2; // double, N=4
    Matrix<int, 2> m3;  // int, N=2
    std::cout << "Matrix m3 (2x2):\n";
    m3.print();
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
    std::cout << "=== Demo NTTP with integer ===\n";
    demoIntegerNTTP();

    std::cout << "\n=== Demo NTTP with bool ===\n";
    demoBoolNTTP();

    std::cout << "\n=== Demo NTTP with enum class ===\n";
    demoEnumNTTP();

    std::cout << "\n=== Demo default type argument ===\n";
    demoDefaultTypeArg();

    std::cout << "\n=== Demo default NTTP ===\n";
    demoDefaultNTTP();

    std::cout << "\n=== Demo combined NTTP + type defaults ===\n";
    demoCombinedNTTP();

    return 0;
}
