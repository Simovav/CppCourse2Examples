#include <iostream>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>

int main() {
    // -----------------------------
    // std::multiset - ordered duplicates
    // -----------------------------
    std::multiset<int> ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(5); // duplicate allowed

    std::cout << "multiset elements: ";
    for (auto x : ms) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "count of 5: " << ms.count(5) << "\n";

    auto it = ms.find(5);
    if (it != ms.end()) {
        std::cout << "erasing one 5\n";
        ms.erase(it); // remove only one 5
    }

    std::cout << "after erase: ";
    for (auto x : ms) std::cout << x << " ";
    std::cout << "\n\n";

    // -----------------------------
    // std::unordered_multiset - unordered duplicates
    // -----------------------------
    std::unordered_multiset<int> ums;
    ums.insert(10);
    ums.insert(20);
    ums.insert(10);

    std::cout << "unordered_multiset elements: ";
    for (auto x : ums) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "count of 10: " << ums.count(10) << "\n\n";

    // -----------------------------
    // std::multimap - ordered duplicates
    // -----------------------------
    std::multimap<std::string, int> mm;
    mm.insert({ "Alice", 10 });
    mm.insert({ "Bob", 20 });
    mm.insert({ "Alice", 30 }); // duplicate key

    std::cout << "multimap values for Alice: ";
    auto range = mm.equal_range("Alice");
    for (auto it = range.first; it != range.second; ++it)
        std::cout << it->second << " ";
    std::cout << "\n";

    mm.erase("Alice"); // remove all Alice entries
    std::cout << "after erase Alice, multimap keys: ";
    for (auto& [k, v] : mm) std::cout << k << " ";
    std::cout << "\n\n";

    // -----------------------------
    // std::unordered_multimap - unordered duplicates
    // -----------------------------
    std::unordered_multimap<std::string, int> umm;
    umm.insert({ "Charlie", 100 });
    umm.insert({ "Charlie", 200 });
    umm.insert({ "Dave", 300 });

    std::cout << "unordered_multimap values for Charlie: ";
    auto urange = umm.equal_range("Charlie");
    for (auto it = urange.first; it != urange.second; ++it)
        std::cout << it->second << " ";
    std::cout << "\n";

    return 0;
}
