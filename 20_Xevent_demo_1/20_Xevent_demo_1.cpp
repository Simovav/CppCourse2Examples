#include <iostream>
#include <vector>
#include <functional>
#include <utility>

template<typename... CallbackSignature>
class xevent {
	std::vector<std::function<void(CallbackSignature...)>> callbacks;

public:
	template<typename Fun>
	void add(Fun&& fun) {
		callbacks.emplace_back(std::forward<Fun>(fun));
	}

	// ===============================================================
	// BUGGY: perfetto forwarding per rvalue multipli
	// ===============================================================
	template<typename... Args>
	void invoke_buggy(Args&&... args) {
		for (auto& cb : callbacks) {
			cb(std::forward<Args>(args)...);
		}
	}

	// ===============================================================
	// SAFE: passaggio come lvalue per più callback
	// ===============================================================
	template<typename... Args>
	void invoke_fixed(Args&&... args) {
		if (callbacks.size() == 1) {
			callbacks[0](std::forward<Args>(args)...); // single callback: forward
		}
		else {
			for (auto& cb : callbacks) {
				cb(args...);
			}
		}
	}

	void clear() { callbacks.clear(); }
};

// ===============================================================
// Handler per vector
// ===============================================================
struct Handler {
	void printVector(const std::vector<int>& v) {
		std::cout << "Handler received vector of size: " << v.size() << "\n";
		for (int n : v) std::cout << n << " ";
		std::cout << "\n";
	}
};

int main() {
	std::cout << "=== xevent with int lambdas ===\n";
	xevent<int> evInt;
	evInt.add([](int v) { std::cout << "Lambda 1: " << v << "\n"; });
	evInt.add([](int v) { std::cout << "Lambda 2: " << v << "\n"; });
	evInt.invoke_fixed(42);
	evInt.clear();

	std::cout << "\n=== xevent with vector and member function ===\n";
	xevent<const std::vector<int>&> evVec;
	Handler h;

	evVec.add([&h](const std::vector<int>& v) { h.printVector(v); });
	evVec.add([](const std::vector<int>& v) {
		std::cout << "Lambda prints first element: " << v[0] << "\n";
		});

	std::vector<int> data = { 1,2,3,4 };
	evVec.invoke_fixed(data); // entrambi leggono vector senza muovere
	std::cout << "After invoke, data.size()=" << data.size() << "\n";

	std::cout << "\n=== Demonstrating buggy invoke with rvalue vector ===\n";
	xevent<std::vector<int>> evMove;
	evMove.add([](std::vector<int> v) {
		std::cout << "Callback1 consumes vector of size: " << v.size() << "\n";
		});
	evMove.add([](std::vector<int> v) {
		std::cout << "Callback2 consumes vector of size: " << v.size() << "\n";
		});

	std::vector<int> vdata = { 10,20,30 };
	evMove.invoke_buggy(std::move(vdata)); // primo callback muove, secondo riceve moved-from vector

	std::cout << "\n=== Safe invoke_fixed with lvalue for multiple callbacks ===\n";
	std::vector<int> vdata2 = { 5,6,7 };
	evMove.invoke_fixed(vdata2); // entrambi ricevono lvalue e copiano

	return 0;
}
