
template<class T> struct A : T {
  constexpr A(T v) : T{v} { }
  // ~A() = default; // Fixes GCC.
};

struct B {
  virtual ~B() = default;
};

inline constexpr auto x = A{B{}};
