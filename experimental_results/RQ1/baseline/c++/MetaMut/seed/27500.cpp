
struct N {
  constexpr N() {}
  N(N const&) = delete;
};

template<typename T> constexpr void bad_assert_copyable() { T t; T t2 = t; }
using ineffective = decltype(bad_assert_copyable<N>());

template<typename T> consteval void assert_copyable() { T t; T t2 = t; }
using check = decltype(assert_copyable<N>());
