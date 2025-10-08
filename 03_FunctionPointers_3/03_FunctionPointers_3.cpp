#include <iostream>

// Define basic arithmetic functions
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; } // simple check for division

int main()
{
    // Array of function pointers
    using fPtr = int(*)(int, int);
    fPtr operations[] = { add, sub, mul, divide };
    const char* names[] = { "add", "sub", "mul", "divide" };

    int a, b, choice;

    std::cout << "Enter two integers: ";
    std::cin >> a >> b;

    std::cout << "\nSelect an operation:\n";
    for (int i = 0; i < 4; ++i)
    {
        std::cout << i << " - " << names[i] << "\n";
    }

    std::cout << "Your choice: (0, 1, 2, 3)";
    std::cin >> choice;

    if (choice >= 0 && choice < 4)
    {
        int result = operations[choice](a, b);
        std::cout << "Result of " << names[choice] << "(" << a << ", " << b << "): " << result << "\n";
    }
    else
    {
        std::cout << "Invalid choice!\n";
    }

    return 0;
}
