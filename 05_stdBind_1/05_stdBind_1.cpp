#include <iostream>
#include <functional> // for std::bind and std::placeholders

// A simple function with three parameters
int compute(int a, int b, int c)
{
    return a + 2 * b + 3 * c;
}

int main()
{
    // Bind the first parameter to 10, leave the other two as placeholders
    auto boundFunc1 = std::bind(compute, 10, std::placeholders::_1, std::placeholders::_2);

    // Call boundFunc1 with two arguments (they will replace _1 and _2)
    int result1 = boundFunc1(5, 2); // equivalent to compute(10, 5, 2)
    std::cout << "Result of boundFunc1(5, 2): " << result1 << "\n";

    // Bind the first two parameters, leave the last one as placeholder
    auto boundFunc2 = std::bind(compute, 1, 2, std::placeholders::_1);

    int result2 = boundFunc2(3); // equivalent to compute(1, 2, 3)
    std::cout << "Result of boundFunc2(3): " << result2 << "\n";

    // Swap order of placeholders
    auto boundFunc3 = std::bind(compute, std::placeholders::_2, std::placeholders::_1, 5);

    int result3 = boundFunc3(7, 4); // equivalent to compute(4, 7, 5)
    std::cout << "Result of boundFunc3(7, 4): " << result3 << "\n";

    return 0;
}
