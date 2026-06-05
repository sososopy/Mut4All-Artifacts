
struct A {
  virtual void f();
};
struct B : A {
  void f() final;
};

