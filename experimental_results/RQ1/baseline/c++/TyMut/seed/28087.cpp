
#include <stdio.h>

struct X {
  unsigned long long a: 1;
} x;

void foo()
{
  printf("%d", x.a);
}

