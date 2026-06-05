
export module mod;

template<class T> struct A : T {
  constexpr A(T v) : T{v} { }
  ~A() = default; // Fixes GCC.
};

struct B {
  virtual ~B() = default;
};

export inline constexpr auto x = A{B{}};

export constexpr const A<B>* y(const B& b) {
  return dynamic_cast<const A<B>*>(&b);
}

import mod;
static_assert(&x == y(x));
int main() { }
