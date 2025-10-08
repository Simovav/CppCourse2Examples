#include <iostream>

class Calculator
{
public:
    int add(int a, int b)
    {
        return a + b;
    }

    int sub(int a, int b)
    {
        return a - b;
    }

    int mul(int a, int b)
    {
        return a * b;
    }
};

int main()
{
    Calculator calc;

    // Pointer to member function syntax: return_type (Class::*)(parameters)
    using MethodPtr = int(Calculator::*)(int, int);

    MethodPtr ptr = nullptr;

    // Assign the add method to the pointer
    ptr = &Calculator::add;

    // Call the method through the pointer on an instance of the class
    int result = (calc.*ptr)(10, 5); // note the syntax: (object.*pointer)(args)
    std::cout << "Result of add(10, 5) through pointer: " << result << "\n";

    // Assign the sub method to the pointer
    ptr = &Calculator::sub;
    result = (calc.*ptr)(10, 5);
    std::cout << "Result of sub(10, 5) through pointer: " << result << "\n";

    // Assign the mul method to the pointer
    ptr = &Calculator::mul;
    result = (calc.*ptr)(10, 5);
    std::cout << "Result of mul(10, 5) through pointer: " << result << "\n";

    return 0;
}
