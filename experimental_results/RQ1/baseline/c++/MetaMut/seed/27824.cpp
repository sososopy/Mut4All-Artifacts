
#include <array>

constexpr char wildcard = '*';

struct Foo {
  char ch_ = wildcard;
};

void func(Foo value) {
  std::array<Foo, 3> x = {value};
}
