
inline int *f() {
  struct {
    int* f() {
      static int i;
      return &i;
    }
  } s;
  return s.f();
}


#include "f.h"
int *x() {
  return f();
}


#include <iostream>
#include "f.h"
int *x();
int *y() {
  return f();
}
int main() {
  std::cout << x() << ", " << y() << '\n';
}

