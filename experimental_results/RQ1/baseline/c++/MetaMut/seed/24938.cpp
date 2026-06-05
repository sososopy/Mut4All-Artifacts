

#include <cstdio>

struct foo {
	foo(double);
	foo(float);
	explicit foo(unsigned);

	unsigned v;
};

void bar(foo x = 0u)
{
	printf("%d\n", x.v);
}

int main()
{
	bar();
	return 0;
}