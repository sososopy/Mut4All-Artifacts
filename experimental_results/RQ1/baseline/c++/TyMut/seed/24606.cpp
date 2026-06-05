
#include <stdio.h>
constexpr int array_length(int (&array)[3]) { return 3; }
int a[] = { 1, 2, 3 };
template <typename T> int f() {
  struct { int e[array_length(a)]; } t;
  return sizeof(t);
}
int main() { printf("%d\n", f<void>()); }
