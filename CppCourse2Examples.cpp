#include <iostream>

// define two simple functions: add and sub
int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int main()
{
    std::cout << "Functions are blocks of code with an address in memory\n";
    std::cout << "You can call them by their name\n";

    std::cout << "You can also get their address and call them through a pointer\n";

    std::cout << std::endl;

    std::cout << "The address of the add function is: " << (void*)&add << "\n";
    std::cout << "The address of the sub function is: " << (void*)&sub << "\n";

    std::cout << std::endl;

    // declare a function pointer for functions that take two int parameters and return an int
    using fPtr = int(*)(int, int);
    fPtr ptr{ nullptr };
    std::cout << "The address of the function pointer is currently: " << (void*)ptr << "\n";
    std::cout << std::endl;

    // assign the address of the add function to the function pointer
    ptr = &add;
    std::cout << "The address of the function pointer after assigning add is: " << (void*)ptr << "\n";

    // call the add function through the function pointer
    int result = ptr(5, 3);
    std::cout << "Result of add(5, 3) through function pointer: " << result << "\n";

    // explicit dereference of the function pointer (equivalent)
    int result2 = (*ptr)(5, 3);
    std::cout << "Result of add(5, 3) through dereferenced function pointer: " << result2 << "\n";

    std::cout << std::endl;

    // assign the address of the sub function to the function pointer
    ptr = &sub;
    std::cout << "The address of the function pointer after assigning sub is: " << (void*)ptr << "\n";

    // call the sub function through the function pointer
    result = ptr(5, 3);
    std::cout << "Result of sub(5, 3) through function pointer: " << result << "\n";

    std::cout << std::endl;
    std::cout << "Even the main function has an address: ";
    std::cout << (void*)&main << "\n";
    std::cout << "But you should never try to call it through a function pointer\n";
    std::cout << "It allows the execution of the program to jump to the main function again\n";

    return 0;
}
