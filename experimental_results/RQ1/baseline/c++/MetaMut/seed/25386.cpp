
#include <sys/resource.h>

void foo(int k) {
	const int size1 = 64;
	const int size2 = 256;
	volatile int t[size1];

	// new[] should throw an exception as soon as memory is exhausted
	int volatile* p = new int[size2];
	t[size1 - 1] = p[size2 - 1] = ++k; // prevent compiler from erasing t and p
	foo(++k);
}

int main() {
	// Set low memory limit
	struct rlimit limit;
	limit.rlim_cur = limit.rlim_max = 10 << 20;
	if (setrlimit(RLIMIT_AS, &limit))
		return 1;
	if (setrlimit(RLIMIT_STACK, &limit))
		return 2;

	foo(0);
	return 0;
}
