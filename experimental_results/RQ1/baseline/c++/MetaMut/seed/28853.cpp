
#include <stdlib.h>
#include <assert.h>

static bool flag = false;

class C
{
  bool prev;

public:
  C() : prev(flag)
  {
    flag = true; // #1
  }

  ~C() {
    flag = prev;
  }
};

void* operator new(unsigned long size)
{
  assert(flag);  // #2
  return malloc(size);
}

void operator delete(void *p)
{
  free(p);
}

void g(int* p)
{
  delete p;
}

void f()
{
  int* p;
  {
    C c;
    p = new int;
  }
  g(p);
}

int main(int, char**)
{
  f();
}
