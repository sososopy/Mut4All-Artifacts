
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static const char *vector_swizzle(int vecsize, int index)
{
	static const char *swizzle[4][4] = {
		{ ".x", ".y", ".z", ".w" },
		{ ".xy", ".yz", ".zw", nullptr },
		{ ".xyz", ".yzw", nullptr, nullptr },
		{ "", nullptr, nullptr, nullptr },
	};

	assert(vecsize >= 1 && vecsize <= 4);
	assert(index >= 0 && index < 4);
	assert(swizzle[vecsize - 1][index]);

	return swizzle[vecsize - 1][index];
}

int main(int argc, char **argv)
{
	puts(vector_swizzle(atoi(argv[1]), atoi(argv[2])));
}

