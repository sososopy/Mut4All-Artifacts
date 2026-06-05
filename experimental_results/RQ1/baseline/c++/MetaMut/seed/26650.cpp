
#include <string>
#include <cassert>

struct test
{
	union {int v = 0;};
	std::string dummy;
};

int main ()
{
	test x;
	x.v = 1;
	test y = x;
	assert (y.v == 1);
}

