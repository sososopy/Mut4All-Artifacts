

template <bool> struct A;
template <bool _Cond> using enable_if_t = typename A<_Cond>::type;
template <typename, int, typename, typename> struct B;
template <int> struct C;
template <typename> struct D;
template <typename Scalar_, int Size_, typename Derived>
struct B<Scalar_, Size_, Derived, enable_if_t<C<Size_>::peel>> {
  using Base = D<Scalar_>;
//  typedef  D<Scalar_>  Base; // OK
  using typename Base::Scalar;
  Scalar a;
};

