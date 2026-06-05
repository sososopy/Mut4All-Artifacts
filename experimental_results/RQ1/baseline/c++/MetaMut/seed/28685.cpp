
#include <array>

struct t
{
	int a;
	t() : a(0) {}
};

std::array<t, X> g;

int main()
{
	g = {};
}

