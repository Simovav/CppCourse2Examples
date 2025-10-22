#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <cmath>

// -----------------------------
// POINT_3D e hash/equal personalizzati
// -----------------------------
struct POINT_3D { double x, y, z; };

struct PointHash {
    size_t operator()(const POINT_3D& p) const {
        auto hx = static_cast<long long>(std::llround(p.x * 1000));
        auto hy = static_cast<long long>(std::llround(p.y * 1000));
        auto hz = static_cast<long long>(std::llround(p.z * 1000));
        size_t hash = 2166136261U;
        hash = 16777619U * hx ^ hash;
        hash = 16777619U * hy ^ hash;
        hash = 16777619U * hz ^ hash;
        return hash;
    }
};

struct PointEqual {
    bool operator()(const POINT_3D& a, const POINT_3D& b) const {
        return std::llround(a.x * 1000) == std::llround(b.x * 1000) &&
            std::llround(a.y * 1000) == std::llround(b.y * 1000) &&
            std::llround(a.z * 1000) == std::llround(b.z * 1000);
    }
};

// -----------------------------
// Transparent Hash/Equal per string usando string_view
// -----------------------------
struct TransparentHash {
    using is_transparent = void;
    size_t operator()(std::string_view s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};

struct TransparentEqual {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const noexcept {
        return a == b;
    }
};

// -----------------------------
// Main
// -----------------------------
int main() {
    std::cout << "=== unordered_set base ===\n";
    std::unordered_set<std::string> names = { "Alice", "Beppe", "Charlie" };
    names.insert("Diana");
    names.erase("Alice");
    for (auto& n : names) std::cout << n << ' ';
    std::cout << "\n\n";

    std::cout << "=== unordered_set con POINT_3D ===\n";
    std::unordered_set<POINT_3D, PointHash, PointEqual> points;
    points.insert({ 0,0,0 });
    points.insert({ 1,1,1 });
    for (auto& p : points)
        std::cout << "(" << p.x << "," << p.y << "," << p.z << ")\n";
    std::cout << "\n";

    std::cout << "=== unordered_set con transparent lookup (string_view) ===\n";
    std::unordered_set<std::string, TransparentHash, TransparentEqual> tnames;
    tnames.insert("Alice");
    tnames.insert("Beppe");

    if (auto it = tnames.find("Alice"); it != tnames.end())
        std::cout << *it << " found\n";  // lookup con const char* senza allocazione
    if (tnames.count("Charlie") == 0)
        std::cout << "Charlie not found\n";
    std::cout << "\n";

    std::cout << "=== unordered_map base ===\n";
    std::unordered_map<std::string, int> ageMap = { {"Alice",30}, {"Beppe",25} };
    ageMap.insert({ "Charlie",28 });
    ageMap.erase("Beppe");
    for (auto& [name, age] : ageMap)
        std::cout << name << ": " << age << "\n";
    std::cout << "\n";

    std::cout << "=== unordered_map con transparent lookup (string_view) ===\n";
    std::unordered_map<std::string, int, TransparentHash, TransparentEqual> tAgeMap;
    tAgeMap["Alice"] = 30;
    tAgeMap["Beppe"] = 25;

    if (auto it = tAgeMap.find("Alice"); it != tAgeMap.end())
        std::cout << it->first << " -> " << it->second << "\n";  // lookup senza allocazione

    for (auto& [name, age] : tAgeMap)
        std::cout << name << ": " << age << "\n";

    return 0;
}
