


#include <iostream>
#include <functional>

int main()
{
	static int a;
	std::function<void(int)> f = [](auto) { std::cout << a << '\n'; };
	a = 1;
	f(0);
}

