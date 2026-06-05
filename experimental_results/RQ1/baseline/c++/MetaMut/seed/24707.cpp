
#include <stdio.h>

struct S {
  S(int) { printf("S(int)\n"); }
  ~S() { printf("~S\n"); }
};

void f(S) { printf("f(S)\n"); }
void h() { printf("h()\n"); }

int main()
{
  f({0}), h();
}
