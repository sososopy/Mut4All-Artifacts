
#include <functional>

class A {
public:
	bool a;
	static inline std::function<void(A&)> foo = std::function<void(A&)>([] (A& o) {
		bool A::*ptr = &A::a;
		o.*ptr = true;
	});
};

int main() {
	A a;
	A::foo(a);
}

