template <typename>
constexpr bool C = true;
template<class T1, typename T2>
struct Foo {
  template<class V> requires C<V> // Index of V in the Foo deduction guide is 2
  Foo(V, T1);
};

template<class T3>
using AFoo = Foo<T3, T3>;
AFoo afoo{0, 1};