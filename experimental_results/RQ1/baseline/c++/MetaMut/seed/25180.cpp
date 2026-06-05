

#include <initializer_list>

class A {
public:
	int x;
	int xplus1() { return x+1; }
	int xplus2() { return x+2; }
};

int main()
{
	const auto members = { &A::xplus1, &A::xplus2 };
	
	A a;
	a.x = 12;
	int y;
	for (auto member : members)
		y = (a.*member)();
	return 0;
}

