
class A {
  virtual void m_fn1();
};
class C {
public:
  virtual void m_fn2();
};
class B : A, public C {};
main() {
  B b;
  static_cast<C *>(&b)->m_fn2();
}
