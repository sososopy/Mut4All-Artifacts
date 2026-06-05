
#include <iostream>
#include <type_traits>

struct X {
	X(const X&) = delete;
	void operator=(X x);
};

struct Y: X {
};

int main()
{
	std::cout << std::is_assignable<X&,X>::value << std::endl;
	std::cout << std::is_assignable<Y&,Y>::value << std::endl;
}

