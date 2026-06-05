
class A {
protected:
  void m_fn1();
};
template <class> struct B : A {
  void m_fn2() const { m_fn1(); }
};

