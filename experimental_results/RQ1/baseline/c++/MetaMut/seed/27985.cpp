
#include <cstdint>

struct S {
  int a;

  constexpr S();
  explicit constexpr S(int a_) : a(a_) {}
};

constexpr S b = S(12);

template <const S& e>
bool c(unsigned int d) {
  return d >= e.a;
}

bool test(unsigned int d);
bool test(unsigned int d) {
    return c<b>(d);
}
