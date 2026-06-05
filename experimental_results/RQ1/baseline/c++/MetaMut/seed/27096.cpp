
#include <stdatomic.h>

int foo (int *p)
{
    return atomic_load (p) < 0;
}

