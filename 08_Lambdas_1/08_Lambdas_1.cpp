#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::cout << "=== 1) Simple lambda without capture ===\n";
    auto simpleLambda = [](int x, int y) { return x + y; };
    std::cout << "simpleLambda(2, 3) = " << simpleLambda(2, 3) << "\n\n";

    std::cout << "=== 2) Lambda capturing by copy ===\n";
    int a = 10;
    auto copyLambda = [a]() { return a * 2; }; // capture 'a' by value
    std::cout << "copyLambda() = " << copyLambda() << "\n";
    a = 20; // changing 'a' outside does not affect the lambda
    std::cout << "copyLambda() after changing 'a' = " << copyLambda() << "\n\n";

    std::cout << "=== 3) Mutable lambda capturing by copy ===\n";
    int b = 5;
    auto mutableLambda = [b]() mutable {
        b += 10;       // modifies its internal copy of b
        return b;
        };
    std::cout << "mutableLambda() first call = " << mutableLambda() << "\n";
    std::cout << "mutableLambda() second call = " << mutableLambda() << "\n";
    std::cout << "Original b outside lambda = " << b << "\n\n";

    std::cout << "=== 4) Lambda capturing by reference ===\n";
    int c = 3;
    auto refLambda = [&c]() { c += 5; return c; };
    std::cout << "refLambda() first call = " << refLambda() << "\n";
    std::cout << "refLambda() second call = " << refLambda() << "\n";
    std::cout << "Original c outside lambda = " << c << "\n\n";

    std::cout << "=== 5) Lambda with capture list initialization ===\n";
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    auto lambdaWithInit = [myVec = vec]() {
        std::cout << "Inside lambda, myVec: ";
        for (int v : myVec) std::cout << v << " ";
        std::cout << "\n";
        };
    lambdaWithInit(); // prints a copy of vec
    vec.push_back(6); // modifying vec outside does not affect myVec
    lambdaWithInit(); // still prints original {1,2,3,4,5}
    std::cout << "\n";

    std::cout << "=== 6) Default capture by reference, mixed captures ===\n";
    int x = 10, y = 20;
    auto mixedLambda = [&x, y, z = x + y]() {
        x += 1; // captured by reference
        std::cout << "x = " << x << ", y = " << y << ", z = " << z << "\n";
        };
    mixedLambda();
    mixedLambda();
    std::cout << "Original x after lambda calls = " << x << "\n";

    return 0;
}
