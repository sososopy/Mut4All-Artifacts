
#include <iostream>

#define CONSTEXPR constexpr

struct foo {
	CONSTEXPR char const *bar() const { return "const"; }
	CONSTEXPR char const *bar() { return "non-const"; }
};

int main() {
	foo f;
	std::cout << f.bar() << std::endl;
	
	foo const b;
	std::cout << b.bar() << std::endl;
	
	return 0;
}
