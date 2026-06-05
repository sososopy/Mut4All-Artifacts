
template <typename T> void *f(T) {
  static int n;
  return &n;
}
inline void *g() {
  struct S {
  } s;
  return reinterpret_cast<void *>(f<S>);
}
void *h();

#include "test.h"
void *h() {
  return g();
}

#include "test.h"
#include <stdio.h>
int main() {
  printf("%p %p\n", g(), h());
}
