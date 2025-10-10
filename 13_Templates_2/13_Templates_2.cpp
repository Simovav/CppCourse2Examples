#include <iostream>

// -----------------------------
// Class template example
// -----------------------------
template <typename T>
class Box {
private:
    T value;
public:
    // Constructor
    Box(T v) : value(v) {}

    // Regular method
    T get() const {
        return value;
    }

    // Template method inside class template
    // NOT instantiated until it is actually used
    template <typename U>
    U multiply(U factor) const {
        return value * factor;
    }
};

// -----------------------------
// Non-template class with template method
// -----------------------------
class Printer {
public:
    // Template method, instantiated only when called
    template <typename T>
    void print(T value) const {
        std::cout << "Printing: " << value << "\n";
    }
};

// -----------------------------
// Explicit template instantiation (outside main)
// -----------------------------

// Force compiler to instantiate Box<char> even if we don't use it in main
template class Box<char>;

// Force compiler to instantiate multiply<double> for Box<float> and Box<float>
template double Box<float>::multiply(double) const;

// Force compiler to instantiate print<float> for Printer
template void Printer::print<float>(float) const;

int main() {
    std::cout << "=== Class Templates Example ===\n\n";

    Box<int> intBox(5);        // Box<int> instantiation
    Box<double> doubleBox(3.14); // Box<double> instantiation

    std::cout << "intBox.get() = " << intBox.get() << "\n";
    std::cout << "doubleBox.get() = " << doubleBox.get() << "\n";

    std::cout << "\nUsing template method multiply<U>:\n";

    std::cout << "intBox.multiply(2) = " << intBox.multiply(2) << "\n";       // multiply<int> instantiated
    std::cout << "intBox.multiply(2.5) = " << intBox.multiply(2.5) << "\n";   // multiply<double> instantiated
    std::cout << "doubleBox.multiply(3) = " << doubleBox.multiply(3) << "\n"; // multiply<int> instantiated
    std::cout << "doubleBox.multiply(1.5) = " << doubleBox.multiply(1.5) << "\n"; // multiply<double> instantiated

    // -----------------------------
    std::cout << "\n=== Non-template Class with Template Method ===\n\n";

    Printer p;

    p.print(42);          // print<int> instantiated
    p.print(3.14);        // print<double> instantiated
    p.print("Hello C++"); // print<const char*> instantiated

    return 0;
}
