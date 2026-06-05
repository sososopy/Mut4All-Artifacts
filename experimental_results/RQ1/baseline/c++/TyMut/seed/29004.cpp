
struct A {
  constexpr A() { c[0] = 0; }
  char c[2];
};

constexpr A a;
