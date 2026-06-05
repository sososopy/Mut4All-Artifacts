
struct A {
  void operator==(const A &);
};
class B {
public:
  A m_fn1();
  A m_fn2();
};
template <typename T, typename M> class C {
public:
  T Key;
  const M &m_fn2(const T &);
  virtual void m_fn1() {}
  B _map;
};

C<int, int> b;
template <typename T, typename M> const M &C<T, M>::m_fn2(const T &) {
  A a = _map.m_fn2();
  a == _map.m_fn1();
  m_fn1();
}

void fn1() { b.m_fn2(0); }

