
struct b {};
struct j : b {
  int e;
  constexpr j(b k) : b(k), e() {}
};
struct g {
  j h;
  constexpr g() : h({}) {}
} a;
