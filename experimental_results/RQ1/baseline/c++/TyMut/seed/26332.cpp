
typedef enum {} nsresult;
class B {
  void *mMappedMemory;

public:
  virtual int m_fn1();
};
class C : public virtual B {};
class D : C {
  virtual nsresult m_fn2();
};
nsresult D::m_fn2() {
  switch (0)
  case 0:
  m_fn1();
}
