
#include <memory>
#include <cassert>
struct A {
  virtual void foo();
};

struct B  : A{ 
  void foo();
};
void f() {
  A *a = new A;
  a->foo();
  A* b = new(&a) B;
  assert(a == b);
  b->foo();
  new(b) A;
  a->foo();
}
