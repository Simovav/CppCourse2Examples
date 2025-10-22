#include <iostream>
#include <vector>
#include <algorithm>

// Dummy geometric object
struct CGeomObject {
    int id;
    void draw() const { std::cout << "Drawing object " << id << "\n"; }
    int area() const { return id * id; } // just for sorting demo
};

// -----------------------------
// Example: Class with Forward Iterator
// -----------------------------
class GeometricGroup {
public:
    GeometricGroup(std::initializer_list<CGeomObject> objs) : objects(objs) {}

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = CGeomObject;
        using difference_type = std::ptrdiff_t;
        using pointer = CGeomObject*;
        using reference = CGeomObject&;

        Iterator(pointer ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(objects.data()); }
    Iterator end() { return Iterator(objects.data() + objects.size()); }

private:
    std::vector<CGeomObject> objects;
};

// -----------------------------
// Forward Iterator Demo
// -----------------------------
void demoForwardIterator() {
    GeometricGroup group{ { {1}, {2}, {3} } };

    std::cout << "Forward iteration:\n";
    for (auto& obj : group) {
        obj.draw();
    }

    std::for_each(group.begin(), group.end(), [](CGeomObject& obj) {
        std::cout << "Area = " << obj.area() << "\n";
        });
}

// -----------------------------
// Random Access Iterator Example
// -----------------------------
class GeometricGroupRA {
public:
    GeometricGroupRA(std::initializer_list<CGeomObject> objs) : objects(objs) {}

    struct RandomAccessIterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = CGeomObject;
        using difference_type = std::ptrdiff_t;
        using pointer = CGeomObject*;
        using reference = CGeomObject&;

        RandomAccessIterator(pointer ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        RandomAccessIterator& operator++() { ++m_ptr; return *this; }
        RandomAccessIterator operator++(int) { RandomAccessIterator tmp = *this; ++(*this); return tmp; }
        RandomAccessIterator& operator--() { --m_ptr; return *this; }
        RandomAccessIterator operator--(int) { RandomAccessIterator tmp = *this; --(*this); return tmp; }
        RandomAccessIterator operator+(difference_type n) const { return RandomAccessIterator(m_ptr + n); }
        RandomAccessIterator operator-(difference_type n) const { return RandomAccessIterator(m_ptr - n); }
        difference_type operator-(const RandomAccessIterator& other) const { return m_ptr - other.m_ptr; }
        reference operator[](difference_type n) const { return *(m_ptr + n); }

        friend bool operator==(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr == b.m_ptr; }
        friend bool operator!=(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr != b.m_ptr; }
        friend bool operator<(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr < b.m_ptr; }
        friend bool operator>(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr > b.m_ptr; }
        friend bool operator<=(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr <= b.m_ptr; }
        friend bool operator>=(const RandomAccessIterator& a, const RandomAccessIterator& b) { return a.m_ptr >= b.m_ptr; }

    private:
        pointer m_ptr;
    };

    RandomAccessIterator begin() { return RandomAccessIterator(objects.data()); }
    RandomAccessIterator end() { return RandomAccessIterator(objects.data() + objects.size()); }

private:
    std::vector<CGeomObject> objects;
};

// -----------------------------
// Random Access Demo
// -----------------------------
void demoRandomAccessIterator() {
    GeometricGroupRA group{ { {3}, {1}, {2} } };
    std::cout << "Random access iteration:\n";
    auto it = group.begin();
    it[1].draw(); // Should draw object with id=1

    std::sort(group.begin(), group.end(), [](const CGeomObject& a, const CGeomObject& b) {
        return a.area() < b.area();
        });

    std::cout << "After sorting:\n";
    for (auto& obj : group) obj.draw();
}

// -----------------------------
// Main
// -----------------------------
int main() {
    demoForwardIterator();
    demoRandomAccessIterator();
    return 0;
}
