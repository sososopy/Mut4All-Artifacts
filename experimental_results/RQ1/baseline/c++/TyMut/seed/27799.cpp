
#include <stdio.h>

class A {
public:
static constexpr int a[2] = {1, 2};
static int m(int x) {
return a[x];
}
};

int main() {
  printf("R=%d", A::m(1));
  return 0;
}
