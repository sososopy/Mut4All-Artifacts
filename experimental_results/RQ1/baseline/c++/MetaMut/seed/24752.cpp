template <typename Derived>
struct MatrixBase {
  template <typename OtherDerived>
  Derived &operator=(const MatrixBase<OtherDerived> &);
};

template <typename>
struct solve_retval;

template <typename Rhs>
struct solve_retval<int> : MatrixBase<solve_retval<Rhs> > {};

void ApproximateChebyshev() {
  MatrixBase<int> c;
  c = solve_retval<int>();
}