

#include <cstdio>

constexpr unsigned int fib(unsigned int N)
{
	return N <= 1 ? 1 : (fib(N - 1) + fib(N - 2));
}

template <unsigned int N>
struct Fib {
	enum { value = fib(N) };
};

int main()
{
	printf("%u\n", Fib<1000>::value);
}

