

  struct A {
      constexpr A() {}
      constexpr operator int() { return 1; }
  };

  template <int> struct X {};

  constexpr A a;
  X<a> x;


