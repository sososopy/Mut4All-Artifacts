
template <int __v> struct A { static constexpr int value = __v; };
template <bool, typename> struct B;
template <typename _Tp> struct B<true, _Tp> { typedef _Tp type; };
template <bool _Cond, typename _Tp>
using enable_if_t = typename B<_Cond, _Tp>::type;
template <typename> constexpr bool array_depth_v = A<1>::value;
template <typename T, int> struct C {
  template <typename Other,
            enable_if_t<array_depth_v<C>, decltype(nullptr)> = nullptr>
  void operator*(Other other) {
    [other](T t) { t *other; };
  }
};

int
main() {
  C<C<float, 4>, 1> a0;
  a0 * 4.f;
  return 0;
}

