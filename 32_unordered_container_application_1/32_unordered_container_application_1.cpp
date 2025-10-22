#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <cmath>
#include <string>

// -----------------------------
// Simple POINT_3D
// -----------------------------
struct POINT_3D {
    double x, y, z;

    POINT_3D(double xx = 0, double yy = 0, double zz = 0)
        : x(xx), y(yy), z(zz) {
    }
};

// -----------------------------
// Truncator for fixed-point comparison
// -----------------------------
template <int Multiplier>
struct Truncator {
    long long operator()(const double& val) const noexcept {
        return static_cast<long long>(std::llround(Multiplier * val));
    }

    bool equal(double a, double b) const noexcept {
        return (*this)(a) == (*this)(b);
    }
};

// -----------------------------
// FixedPoint Hash/Equals
// -----------------------------
template <int Multiplier = 500>
struct FixedPoint3DHasher {
    std::size_t operator()(const POINT_3D& key) const noexcept {
        Truncator<Multiplier> tr;
        size_t hash = 2166136261U;
        hash = 16777619U * tr(key.x) ^ hash;
        hash = 16777619U * tr(key.y) ^ hash;
        hash = 16777619U * tr(key.z) ^ hash;
        return hash;
    }
};

template <int Multiplier = 500>
struct FixedPoint3DEquals {
    bool operator()(const POINT_3D& a, const POINT_3D& b) const noexcept {
        Truncator<Multiplier> tr;
        return tr.equal(a.x, b.x) && tr.equal(a.y, b.y) && tr.equal(a.z, b.z);
    }
};

// -----------------------------
// Simplified PathLooper with Truncator
// -----------------------------
class CPathLooperSimple {
    using Point3DList = std::list<POINT_3D>;
    using FixedPoint3DMap = std::unordered_map<POINT_3D, Point3DList*,
        FixedPoint3DHasher<500>, FixedPoint3DEquals<500>>;

public:
    ~CPathLooperSimple() {
        std::unordered_map<Point3DList*, bool> deleted;
        for (auto& [pt, list] : m_map) {
            if (!deleted[list]) {
                delete list;
                deleted[list] = true;
            }
        }
    }

    void AddLine(const POINT_3D& pt1, const POINT_3D& pt2) {
        Truncator<500> tr;

        auto it1 = m_map.find(pt1);
        auto it2 = m_map.find(pt2);

        if (it1 != m_map.end() && it2 != m_map.end()) {
            // both points exist in different lists -> merge
            if (it1->second != it2->second) {
                Point3DList* list1 = it1->second;
                Point3DList* list2 = it2->second;

                // decide if we need to reverse list2 to match the endpoints
                if (tr.equal(list1->front().x, pt1.x) && tr.equal(list1->front().y, pt1.y) && tr.equal(list1->front().z, pt1.z)) {
                    list2->reverse();
                }
                else if (tr.equal(list1->back().x, pt1.x) && tr.equal(list1->back().y, pt1.y) && tr.equal(list1->back().z, pt1.z)) {
                    // do nothing
                }
                else if (tr.equal(list1->front().x, pt2.x) && tr.equal(list1->front().y, pt2.y) && tr.equal(list1->front().z, pt2.z)) {
                    list1->reverse();
                    list2->reverse();
                }
                else if (tr.equal(list1->back().x, pt2.x) && tr.equal(list1->back().y, pt2.y) && tr.equal(list1->back().z, pt2.z)) {
                    list1->reverse();
                }

                // merge
                list1->splice(list1->end(), *list2);
                for (auto& p : *list2) m_map[p] = list1;
                delete list2;
            }
        }
        else if (it1 != m_map.end()) {
            // pt1 exists -> attach pt2 in correct position
            Point3DList* list = it1->second;
            if (tr.equal(list->front().x, pt1.x) && tr.equal(list->front().y, pt1.y) && tr.equal(list->front().z, pt1.z)) {
                list->push_front(pt2);
            }
            else {
                list->push_back(pt2);
            }
            m_map[pt2] = list;
        }
        else if (it2 != m_map.end()) {
            // pt2 exists -> attach pt1 in correct position
            Point3DList* list = it2->second;
            if (tr.equal(list->front().x, pt2.x) && tr.equal(list->front().y, pt2.y) && tr.equal(list->front().z, pt2.z)) {
                list->push_front(pt1);
            }
            else {
                list->push_back(pt1);
            }
            m_map[pt1] = list;
        }
        else {
            // neither exists -> create new list
            Point3DList* list = new Point3DList{ pt1, pt2 };
            m_map[pt1] = list;
            m_map[pt2] = list;
        }
    }

    void PrintPaths() const {
        std::unordered_map<const Point3DList*, bool> seen;
        for (auto& [pt, list] : m_map) {
            if (seen[list]) continue;
            seen[list] = true;
            std::cout << "Path: ";
            for (auto& p : *list)
                std::cout << "(" << p.x << "," << p.y << "," << p.z << ") ";
            std::cout << "\n";
        }
    }

private:
    FixedPoint3DMap m_map;
};

// -----------------------------
// Demo
// -----------------------------
int main() {
    CPathLooperSimple looper;

    looper.AddLine({ 5,5,5 }, { 4,4,4 });
    looper.AddLine({ 0,0,0 }, { 1,1,1 });
    looper.AddLine({ 1,1,1 }, { 2,2,2 }); // merges with previous
    looper.AddLine({ 3,3,3 }, { 4,4,4 }); // merges with first path

    looper.PrintPaths();
}
