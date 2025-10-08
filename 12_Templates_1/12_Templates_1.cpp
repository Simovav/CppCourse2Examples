#include <iostream>

template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << "add<int>(3, 5) = " << add<int>(3, 5) << "\n";
    std::cout << "add<double>(2.5, 4.1) = " << add<double>(2.5, 4.1) << "\n\n";

    // Print addresses of the template instantiations
    std::cout << "Address of add<int>: " << (void*)&add<int> << "\n";
    std::cout << "Address of add<double>: " << (void*)&add<double> << "\n";

    return 0;
}
