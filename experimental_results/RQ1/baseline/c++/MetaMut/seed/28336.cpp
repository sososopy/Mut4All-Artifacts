
struct A {
  int operator* ();
  void operator++ ();
  bool operator!= (A);
};
struct B {
  A begin ();
  A end ();
};
struct C {
  template <typename>
  void foo ();
};
struct D : public C {
  void foo (int);
  virtual void bar ();
};
template <typename = int> struct E : D {
  using D::foo;
  using C::foo;
  B b;
  void bar () { for (auto i : b) foo (i); }
};
struct F {
  using G = E<>;
  F() {}
  G g;
};
