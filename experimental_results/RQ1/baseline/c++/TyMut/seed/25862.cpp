template<int A, int B> struct S;
template<int A> struct S<A, 0>;   
template<int B> struct S<0, B> {
  S<1, 0> f() { return S<1, 0>(); }
};

