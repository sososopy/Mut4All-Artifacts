
struct X { X(); };

struct A {
  struct B {
    B() noexcept(A::value) = default;
    X x;
  };
  decltype(B()) b;
  static constexpr bool value = true;
};
A::B b;

static_assert(noexcept(A::B()), "");

