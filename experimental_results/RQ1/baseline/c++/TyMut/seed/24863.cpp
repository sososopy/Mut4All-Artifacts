

#include <iostream>

template <bool x>
class B {
public:
	constexpr static bool con = x;
};

template<bool x = true>
class A: public B<x> {
public:
	// Option 1: Incorrectly accepted
	bool c() requires con {
		return true;
	}
	// Option 2: Correct behaviour
	// Doesn't compile: con was not declared
	// bool c() requires (con) {
		// return true;
	// }
};

int main() {
	A<> a{};
	std::cout << a.c() << std::endl;	// 1
	std::cout << a.con << std::endl;	// 1

	// Here the method is chosen based on B<true> not B<false>
	A<false> aa{};
	std::cout << aa.c() << std::endl;	// 1
	std::cout << aa.con << std::endl;	// 0
}

