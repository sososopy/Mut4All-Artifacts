
#include <stdint.h>

typedef int32_t int32x4_t __attribute__((vector_size(16)));

int32x4_t apply_shift(int32x4_t arg)
{
    int32x4_t s = { 1, 1, 1, 1 };

    arg <<= 1;
    arg = arg << 1;
    arg <<= s;
    arg = arg << s;

    return arg;
}

