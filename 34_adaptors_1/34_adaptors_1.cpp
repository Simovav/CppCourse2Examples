#include <iostream>
#include <stack>
#include <queue>
#include <vector>

int main() {
    // -----------------------------
    // std::stack - LIFO
    // -----------------------------
    std::stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "Stack LIFO behavior:\n";
    while (!s.empty()) {
        std::cout << s.top() << " "; // access top
        s.pop();                     // remove top
    }
    std::cout << "\n\n";

    // -----------------------------
    // std::queue - FIFO
    // -----------------------------
    std::queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    std::cout << "Queue FIFO behavior:\n";
    while (!q.empty()) {
        std::cout << q.front() << " "; // access front
        q.pop();                       // remove front
    }
    std::cout << "\n\n";

    // -----------------------------
    // std::priority_queue - Max heap (default)
    // -----------------------------
    std::priority_queue<int> pq;
    pq.push(10);
    pq.push(30);
    pq.push(20);

    std::cout << "Priority queue (max heap) behavior:\n";
    while (!pq.empty()) {
        std::cout << pq.top() << " "; // access largest
        pq.pop();                     // remove largest
    }
    std::cout << "\n\n";

    // -----------------------------
    // std::priority_queue - Min heap using std::greater
    // -----------------------------
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    minHeap.push(10);
    minHeap.push(30);
    minHeap.push(20);

    std::cout << "Priority queue (min heap) behavior:\n";
    while (!minHeap.empty()) {
        std::cout << minHeap.top() << " "; // access smallest
        minHeap.pop();                     // remove smallest
    }
    std::cout << "\n";

    return 0;
}
