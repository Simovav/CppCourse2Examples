#include <iostream>
#include <type_traits>

int main()
{
    // Two lambdas with the same signature
    auto lambda1 = [](int x) { return x + 1; };
    auto lambda2 = [](int x) { return x + 1; };

    // Their types are unique
    std::cout << "Are lambda1 and lambda2 of the same type? "
        << (std::is_same<decltype(lambda1), decltype(lambda2)>::value ? "Yes" : "No") << "\n";

    // Lambda assigned to a variable
    auto lambda3 = lambda1;
    std::cout << "Is lambda3 the same type as lambda1? "
        << (std::is_same<decltype(lambda1), decltype(lambda3)>::value ? "Yes" : "No") << "\n";

    // Check with typeid (just for demonstration)
    std::cout << "Type of lambda1: " << typeid(lambda1).name() << "\n";
    std::cout << "Type of lambda2: " << typeid(lambda2).name() << "\n";
    std::cout << "Type of lambda3: " << typeid(lambda3).name() << "\n";

    return 0;
}
