
template <typename T> struct A {
  constexpr ~A() requires(false) {}
  constexpr ~A() requires(!true) {}
};

constexpr A<int> a;
