
template <bool... B>
struct S {
  constexpr explicit(B) S() {}
};

constexpr S<true> s;

