
struct A {
  constexpr A (int = 0) { }
  constexpr operator int () const { return 0; }
};

constexpr int foo () {
  A a [3] = { 1, 2, 3 };
  return a [99];
}

struct B { unsigned b: foo () + 1; };

#if __GNUG__ > 5
  // GCC 5 fails on this because of either 69509 or 69516
constexpr int bar (int n) {
  A a [n] = { 1, 2, 3 };
  return a [99];
}

struct C {
    unsigned c: bar (1) + 1;
};

#endif
