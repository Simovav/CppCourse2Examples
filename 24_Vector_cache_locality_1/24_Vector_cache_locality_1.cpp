// -----------------------------
// Demo: Cache effects and contiguous memory
// -----------------------------
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <new>
#include <utility>
#include <algorithm>
#include <chrono>

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

class Grid {
    std::vector<int> data_;
    size_t rows_, cols_;
public:
    Grid(size_t r, size_t c) : data_(r* c, 0), rows_(r), cols_(c) {}

    // Access operator converting 2D indices to linear index
    int& at(size_t x, size_t y) {
        return data_[x * cols_ + y]; // row-major layout
    }

    const int& at(size_t x, size_t y) const {
        return data_[x * cols_ + y];
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
};

void demo_cache_effects() {
    std::cout << "\n=== demo_cache_effects ===\n";

    const size_t N = 8000; // 8000x8000 = ~256 MB int array
    Grid g(N, N);

    std::cout << "Grid size: " << N << "x" << N << " (~"
        << (N * N * sizeof(int)) / (1024 * 1024) << " MB)\n";

    // Cache-friendly iteration (row-major)
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < g.rows(); ++i) {
        for (size_t j = 0; j < g.cols(); ++j) {
            g.at(i, j) += 1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_row_major = end - start;
    std::cout << "Row-major iteration time: " << duration_row_major.count() << " s\n";

    // Cache-unfriendly iteration (column-major)
    start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < g.cols(); ++j) {
        for (size_t i = 0; i < g.rows(); ++i) {
            g.at(i, j) += 1;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_col_major = end - start;
    std::cout << "Column-major iteration time: " << duration_col_major.count() << " s\n";

    std::cout << "Note: accessing memory sequentially (row-major) is faster due to cache locality.\n";
}

int main() {
        std::cout << "=== std::vector cache locality demo ===\n";
		demo_cache_effects();
}