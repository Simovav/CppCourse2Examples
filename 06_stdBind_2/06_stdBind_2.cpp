#include <iostream>
#include <functional>

class Calculator
{
private:
	inline static int nextIdValue{ 1 };  // static counter for IDs
	int* id; // dynamic allocation

public:
	Calculator() : id(new int(nextIdValue++)) {}

	~Calculator()
	{
		delete id;
		id = nullptr; // make it explicit
	}

	int getId() const
	{
		return *id; // will crash if id == nullptr
	}

	int add(int a, int b)
	{
		std::cout << "[Calculator ID: " << *id << "] add(" << a << ", " << b << ") called\n";
		return a + b;
	}
};

int main()
{
	std::cout << "=== Correct usage with multiple instances ===\n";
	{
		Calculator calc1;
		Calculator calc2;

		auto boundAdd1 = std::bind(&Calculator::add, &calc1, std::placeholders::_1, std::placeholders::_2);
		auto boundAdd2 = std::bind(&Calculator::add, &calc2, std::placeholders::_1, std::placeholders::_2);

		int result1 = boundAdd1(10, 5);
		int result2 = boundAdd2(3, 7);

		std::cout << "Result from calc1: " << result1 << "\n";
		std::cout << "Result from calc2: " << result2 << "\n";
	}

	std::cout << "\n=== Dangling object example (will crash) ===\n";

	Calculator* calc = new Calculator;

	auto boundAddDangling = std::bind(&Calculator::add, calc, std::placeholders::_1, std::placeholders::_2);

	int result = boundAddDangling(5, 3); // works fine
	std::cout << "Result inside scope: " << result << "\n";

	std::cout << "Deleting object...\n";
	delete calc; // id is deleted and set to nullptr
	calc = nullptr;

	std::cout << "Calling after object is destroyed (will crash due to dereferencing nullptr)\n";
	result = boundAddDangling(2, 2); // CRASH garantito
	std::cout << "Result after destruction: " << result << "\n";

	return 0;
}
