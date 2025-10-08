#include <iostream>

// Stateful functor: keeps track of how many times it has been called
class Counter
{
private:
    int count;

public:
    Counter() : count(0) {}

    // operator() makes this object callable like a function
    int operator()(int value)
    {
        count++;
        std::cout << "Call #" << count << ": adding " << value << "\n";
        return value + count;
    }
};

int main()
{
    Counter counter; // instantiate the functor

    // Use the functor like a function
    int result1 = counter(5);  // first call
    int result2 = counter(5);  // second call
    int result3 = counter(10); // third call

    std::cout << "Results: " << result1 << ", " << result2 << ", " << result3 << "\n";

    return 0;
}
