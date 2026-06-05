
template <typename _Tp> struct integral_constant {
  static constexpr _Tp value = 0;
};
template <bool, typename> struct conditional;
template <typename...> struct A;
template <typename _B1, typename _B2>
struct A<_B1, _B2> : conditional<_B1::value, _B2>::type {};
struct B : integral_constant<bool> {};
struct C {
  template <typename _Tp, typename = decltype(_Tp())>
  static integral_constant<bool> __test(int);
};
template <typename _Tp> struct L : C { typedef decltype(__test<_Tp>(0)) type; };
template <typename _Tp> struct M : A<B, L<_Tp>> {};
template <typename _Tp> struct N : M<_Tp>::type {};
struct D {
  static integral_constant<bool> __test();
};
template <typename> struct __is_implicitly_default_constructible_impl : D {
  typedef decltype(__test()) type;
};
template <typename>
struct F : __is_implicitly_default_constructible_impl<int>::type {};
template <typename _Tp> struct O : A<N<_Tp>, F<_Tp>> {};
template <bool> struct G;
template <bool, typename _Iftrue> struct conditional { typedef _Iftrue type; };
template <typename _T2> struct H {
  template <typename _U1 = int, typename _U2 = _T2,
            typename G<A<O<_U1>, O<_U2>>::value>::type = true>
  H();
};
template <typename ValueT, typename BucketT = H<ValueT>> struct I {
  BucketT storage;
};
struct J {
  struct K {
    int First = 0;
  };
  I<K> FunctionMDInfo;
};

