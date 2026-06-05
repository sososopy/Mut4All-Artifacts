
#include <type_traits>

struct Foo {
  int x;
};

constexpr bool compare(int Foo::*t) { return t == &Foo::x; }

int main(int, char **) {
  // GCC will fail here, "(&Foo::x == 0) is not a constant expression"
  constexpr bool b = compare(&Foo::x);
  return b ? 0 : 1;
}
