
class A {
public:
  virtual ~A();
};
class B : A {
  virtual int m_fn1();
};
void fn1() {
  delete reinterpret_cast<B*>(1);
}
