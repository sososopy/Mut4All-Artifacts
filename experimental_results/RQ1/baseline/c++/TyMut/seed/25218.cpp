

template <char, class Preconditioner> class A {
  template <class = Preconditioner> void m_fn1() { m_fn1(); }
};

