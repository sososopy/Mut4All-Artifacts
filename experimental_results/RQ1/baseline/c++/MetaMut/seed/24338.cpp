
template <int> struct A;
template <int> class B {
  template <int A_S> friend void A<A_S>::impl();
};
B<0> a;

