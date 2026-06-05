

#include <stdint.h>
#include <stdio.h>

struct A {
  A(int32_t a) {}
  A(uint32_t a) {}  // Comment to make it work

  explicit A(int64_t a) {}  // Comment to make it work
};

void f(A a) { printf("hello\n"); }

int main(int, char**) {
  f(2);
  f(3l);
}

