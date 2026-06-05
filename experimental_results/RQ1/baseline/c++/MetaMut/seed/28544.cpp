
constexpr auto foo() {
  struct S { int d; };
  return S{0};
}

inline void bar() {
  constexpr auto t = foo();
}

