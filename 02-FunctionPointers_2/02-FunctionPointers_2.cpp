#include <iostream>

// Define two simple functions that perform basic arithmetic operations
int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

// Define a calculate function that takes two integers and a function pointer as parameters
int calculate(int a, int b, int(*operation)(int, int))
{
    std::cout << "Address of the passed function: " << (void*)operation << "\n";
    return operation(a, b);
}

int main()
{
    // Declare a function pointer for functions taking two int parameters and returning int
    using fPtr = int(*)(int, int);
    fPtr ptr{ nullptr };

    std::cout << "Address of the add function: " << (void*)&add << "\n";
    std::cout << "Address of the sub function: " << (void*)&sub << "\n";
    std::cout << std::endl;

    // Assign the address of the add function to the function pointer
    ptr = &add;

    // Call the calculate function by passing the function pointer
    int result = calculate(5, 3, ptr);
    std::cout << "Result of add(5, 3) through calculate: " << result << "\n";
    std::cout << std::endl;

    // We could also pass the function address directly without using the pointer
    result = calculate(5, 3, &add);
    std::cout << "Result of add(5, 3) through calculate (direct function address): " << result << "\n";
    std::cout << std::endl;

    // Assign the address of the sub function to the function pointer
    ptr = &sub;

    // Call the calculate function by passing the function pointer
    result = calculate(5, 3, ptr);
    std::cout << "Result of sub(5, 3) through calculate: " << result << "\n";

    return 0;
}
