#include <iostream>
#include <memory>
#include <vector>
#include <string>

// ===============================================================
// 1. Manual memory management problem
// ===============================================================
void manualMemory() {
    std::cout << "--- Manual memory management ---\n";
    try {
        int* p = new int(42);
        throw std::runtime_error("oops");
        delete p; // never reached → memory leak!
    }
    catch (...) {
        std::cout << "Exception caught, memory leaked if not careful!\n";
    }
}

// ===============================================================
// 2. unique_ptr basics
// ===============================================================
void uniquePtrDemo() {
    std::cout << "\n--- unique_ptr demo ---\n";

    // create using std::make_unique
    auto ptr = std::make_unique<int>(100);
    std::cout << "*ptr = " << *ptr << "\n";

    // transfer ownership
    std::unique_ptr<int> ptr2 = std::move(ptr);
    std::cout << "ptr after move is " << (ptr ? "not null" : "null") << "\n";
    std::cout << "*ptr2 = " << *ptr2 << "\n";

    // release and reset
    int* raw = ptr2.release(); // ptr2 is null
    std::cout << "raw pointer = " << *raw << "\n";
    delete raw;

    ptr2.reset(new int(50));   // manage new object
    std::cout << "*ptr2 after reset = " << *ptr2 << "\n";
    ptr2.reset(); // deletes managed object
}

// ===============================================================
// 3. unique_ptr with container
// ===============================================================
void uniquePtrVector() {
    std::cout << "\n--- unique_ptr with vector ---\n";
    std::vector<std::unique_ptr<int>> vec;
    vec.push_back(std::make_unique<int>(1));
    vec.push_back(std::make_unique<int>(2));
    vec.push_back(std::make_unique<int>(3));

    for (const auto& p : vec)
        std::cout << *p << " ";
    std::cout << "\n";
}

// ===============================================================
// 4. shared_ptr basics
// ===============================================================
void sharedPtrDemo() {
    std::cout << "\n--- shared_ptr demo ---\n";

    struct Widget {
        Widget(int id) : id(id) { std::cout << "Widget " << id << " created\n"; }
        ~Widget() { std::cout << "Widget " << id << " destroyed\n"; }
        int id;
    };

    auto sp1 = std::make_shared<Widget>(1);
    {
        auto sp2 = sp1; // shared ownership
        std::cout << "Inside scope: use_count = " << sp1.use_count() << "\n";
    } // sp2 goes out of scope
    std::cout << "Outside scope: use_count = " << sp1.use_count() << "\n";
}

// ===============================================================
// 5. weak_ptr basics
// ===============================================================
void weakPtrDemo() {
    std::cout << "\n--- weak_ptr demo ---\n";

    auto sp = std::make_shared<int>(42);
    std::weak_ptr<int> wp = sp; // non-owning

    std::cout << "Before sp reset: wp use_count = " << wp.use_count() << "\n";
    if (auto locked = wp.lock()) {
        std::cout << "Locked value = " << *locked << "\n";
    }

    sp.reset(); // destroy object
    std::cout << "After sp reset: wp use_count = " << wp.use_count() << "\n";

    if (auto locked = wp.lock()) {
        std::cout << "Locked value = " << *locked << "\n";
    }
    else {
        std::cout << "Object no longer exists\n";
    }
}

// ===============================================================
// 6. Breaking cycles with weak_ptr
// ===============================================================
struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev; // prevents cyclic ownership

    Node(std::string n) : name(n) { std::cout << "Node " << name << " created\n"; }
    ~Node() { std::cout << "Node " << name << " destroyed\n"; }
};

void cyclicGraphDemo() {
    std::cout << "\n--- cyclic graph with weak_ptr ---\n";
    auto n1 = std::make_shared<Node>("n1");
    auto n2 = std::make_shared<Node>("n2");

    n1->next = n2;
    n2->prev = n1; // weak_ptr avoids cycle

    std::cout << "Exiting scope, nodes should be destroyed safely...\n";
}

// ===============================================================
// MAIN
// ===============================================================
int main() {
    manualMemory();
    uniquePtrDemo();
    uniquePtrVector();
    sharedPtrDemo();
    weakPtrDemo();
    cyclicGraphDemo();

    return 0;
}
