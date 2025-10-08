#include <iostream>
#include <vector>
#include <functional>
#include <string>

// Logger functor
struct Logger {
    std::string prefix;
    Logger(const std::string& p) : prefix(p) {}
    void operator()(const std::string& message) const {
        std::cout << prefix << message << "\n";
    }
};

// Worker class
class Worker {
private:
    int id;
public:
    Worker(int i) : id(i) {}

    void processTask(int value) const {
        std::cout << "[Worker " << id << "] Processing value: " << value
            << ", squared = " << value * value << "\n";
    }
};

// Free function
void printSum(int x, int y) {
    std::cout << "[Free function] Sum = " << x + y << "\n";
}

int main() {
    std::cout << "=== Dynamic Event Loop with Nested Scheduling ===\n";

    std::vector<std::function<void()>> eventQueue;

    int counter = 0;

    // 1) Lambda that schedules another task
    eventQueue.push_back([&eventQueue]() {
        std::cout << "[Lambda] scheduling a new task dynamically\n";
        eventQueue.push_back([]() {
            std::cout << "[Dynamically added lambda] Hello from dynamically scheduled task!\n";
            });
        });

    // 2) Stateful functor
    Logger logger("Logger: ");
    eventQueue.push_back(std::bind(logger, "Initial log event"));

    // 3) Free function with bound parameters
    eventQueue.push_back(std::bind(printSum, 3, 4));

    // 4) Member function bound to objects
    Worker w1(1);
    Worker w2(2);
    eventQueue.push_back(std::bind(&Worker::processTask, &w1, 5));
    eventQueue.push_back(std::bind(&Worker::processTask, &w2, 8));

    // 5) Lambda capturing external counter by reference
    eventQueue.push_back([&counter]() {
        counter += 10;
        std::cout << "[Lambda reference] counter = " << counter << "\n";
        });

    // Event loop simulation
    size_t i = 0;
    while (i < eventQueue.size()) {
        eventQueue[i](); // execute task
        ++i;
    }

    std::cout << "Final counter value = " << counter << "\n";

    return 0;
}
