

#include <math.h>
#include <cstdlib>

int
wrap_abs (int x)
{
  return abs (x) + std::abs(x);
}


