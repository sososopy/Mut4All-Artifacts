template <class T> struct A {
  void operator()(T *p1) { delete p1; }
};
template <class D> class B {
public:
  ~B();
  void m_fn1(int *) { static_cast<D &>(data_)(0); }
  struct G : D {};
  G data_;
};

template <class T, class D = A<T> > class C {
public:
  void m_fn1() { impl_.m_fn1(0); }
  B<D> impl_;
};

class D {
public:
  ~D();
  void m_fn1() {}
  C<D> x;
};

class F {
  virtual C<int> m_fn1(C<D>);
};
class H : F {
  C<int> m_fn1(C<D>);
};
C<int> a;
void fn1() { new H; }
