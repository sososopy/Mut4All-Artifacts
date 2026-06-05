

#include <stddef.h>

struct C
{
  int x;
  int y;
};

int main()
{
  int a;
  int b = a + offsetof(C, y); // <= missing warning that 'a' is uninitialized
  return b;
}
