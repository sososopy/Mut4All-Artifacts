
  #include <concepts>
  constexpr auto foo(const auto &A, int i, int j)  requires(requires(decltype(A) a, int ii) { a[ii, ii]; }) {
    return A[i, j];
  }
  constexpr auto foo(const auto &A, int i, int j) {
    return A + i + j;
  }
  static_assert(foo(2,3,4) == 9);


  #include <concepts>
  template <typename T, typename S>
  concept CartesianIndexable = requires(T t, int i) {
    { t[i, i] } -> std::convertible_to<S>;
  };
  static_assert(!CartesianIndexable<double,int>);
