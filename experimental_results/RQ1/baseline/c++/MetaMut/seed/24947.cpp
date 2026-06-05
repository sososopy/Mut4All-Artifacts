
template<class T> struct A {
  struct B { /* ... */ };
  typedef void C;
  void f();
  template<class U> void g(U);
};

template<class B> void A<B>::f() {
  B b;              // A's B, not the template parameter
}

template<class B> template<class C> void A<B>::g(C) {
  B b;              // A's B, not the template parameter
  C c;              // the template parameter C, not A's C
}
