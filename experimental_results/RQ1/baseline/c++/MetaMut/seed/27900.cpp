
#include <new>
int* f (void* a)
{
  return new(a) int();
}

