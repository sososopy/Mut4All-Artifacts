
class A {
protected:
  ~A();

public:
  friend struct C;
  virtual void foo();
};

class B final : public A {
  void foo() override;
};

