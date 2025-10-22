#include <vector>
#include <list>
#include <algorithm>
#include <iostream>

int main() {
    std::cout << "=== Example 1: Erase-Remove with direct value ===\n";
    std::vector<int> v1 = { 1, 2, 3, 4, 2, 5 };

    // Remove all '2's
    auto new_end1 = std::remove(v1.begin(), v1.end(), 2);
    v1.erase(new_end1, v1.end());

    for (int x : v1) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== Example 2: Naive erase in loop (O(n^2)) ===\n";
    std::vector<int> v2 = { 1, 2, 3, 4, 2, 5 };

    for (auto it = v2.begin(); it != v2.end(); ) {
        if (*it == 2)
            it = v2.erase(it); // linear erase
        else
            ++it;
    }

    for (int x : v2) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== Example 3: Erase-Remove with predicate ===\n";
    std::vector<int> v3 = { 1, 2, 3, 4, 2, 5 };

    // Remove all even numbers
    auto new_end3 = std::remove_if(v3.begin(), v3.end(), [](int x) { return x % 2 == 0; });
    v3.erase(new_end3, v3.end());

    for (int x : v3) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== Example 4: Using std::list (no need for erase-remove) ===\n";
    std::list<int> lst = { 1, 2, 3, 4, 2, 5 };

    // std::list::remove removes all matching elements in O(n)
    lst.remove(2);

    for (int x : lst) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== Example 5: Combining remove_if with complex predicate ===\n";
    struct Item { int id; std::string name; };
    std::vector<Item> items = { {1,"A"}, {2,"B"}, {3,"C"}, {2,"D"} };

    // Remove all items with id == 2
    auto new_end5 = std::remove_if(items.begin(), items.end(),
        [](const Item& it) { return it.id == 2; });
    items.erase(new_end5, items.end());

    for (auto& it : items) std::cout << it.id << it.name << " ";
    std::cout << "\n";

    return 0;
}
