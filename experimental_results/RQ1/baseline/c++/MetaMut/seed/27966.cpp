
struct Base {
  virtual ~Base();
};
struct Derived : Base {
  void operator delete(void *) {}
};
void foo() { Derived d1; }
