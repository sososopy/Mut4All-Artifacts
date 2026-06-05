
#include <iostream>
void log(std::string msg) { std::cout << msg << std::endl; }

struct Base
{
	Base() { log(__func__); }
	Base(int, int) { log(__func__); }
};
struct D1 : Base
{
	D1() { log(__func__); }
};
struct D2 : D1
{
	D2() { log(__func__); }
	using Base::Base;
};

int main()
{
	D2 d(1, 2);
}
