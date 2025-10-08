#include <iostream>
#include <vector>
#include <functional>

// Free function
int freeFunction(int x) {
    return x * 2;
}

// Free function with two parameters
int addXY(int x, int y) {
    return x + y;
}

// Stateful functor
struct Multiply {
    int factor;
    Multiply(int f) : factor(f) {}
    int operator()(int x) const {
        std::cout << "[Multiply functor state: factor=" << factor << "] ";
        return x * factor;
    }
};

// Class with member function
class Calculator {
private:
    int id;
public:
    Calculator(int i) : id(i) {}

    int add(int x, int y) const {
        std::cout << "[Calculator ID: " << id << "] ";
        return x + y;
    }
};

int main() {
    std::cout << "=== std::function with heterogeneous callables including stateful lambdas ===\n";

    std::vector<std::function<int(int)>> funcs;

    // 1) Simple lambda
    auto simpleLambda = [](int x) { return x + 1; };
    funcs.push_back(simpleLambda);

    // 2) Stateful lambda (captures local variable by value)
    int capturedValue = 5;
    auto statefulLambda = [capturedValue](int x) mutable {
        capturedValue += 1; // modifies its internal copy
        std::cout << "[Stateful lambda internal value: " << capturedValue << "] ";
        return x + capturedValue;
        };
    funcs.push_back(statefulLambda);

    // 3) Stateful functor
    Multiply multiplier(3);
    funcs.push_back(multiplier);

    // 4) Function pointer
    funcs.push_back(&freeFunction);

    // 5) std::bind on free function
    auto boundFree = std::bind(freeFunction, std::placeholders::_1);
    funcs.push_back(boundFree);

    // 6) std::bind on two-parameter free function, fix second parameter to 5
    auto boundAddXY = std::bind(addXY, std::placeholders::_1, 5);
    funcs.push_back(boundAddXY);

    // 7) std::bind on member function
    Calculator calc1(101);
    auto boundMethod = std::bind(&Calculator::add, &calc1, std::placeholders::_1, 10);
    funcs.push_back(boundMethod);

    // 8) Another stateful lambda (captures by reference)
    int externalValue = 100;
    auto refLambda = [&externalValue](int x) {
        externalValue += x;
        std::cout << "[Stateful lambda reference externalValue: " << externalValue << "] ";
        return x + externalValue;
        };
    funcs.push_back(refLambda);

    // Execute all callables
    int value = 10;
    for (size_t i = 0; i < funcs.size(); ++i) {
        std::cout << "funcs[" << i << "](" << value << ") = " << funcs[i](value) << "\n";
    }

    std::cout << "Final externalValue = " << externalValue << "\n";

    return 0;
}
