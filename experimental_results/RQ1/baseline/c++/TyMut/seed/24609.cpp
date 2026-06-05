
struct A {
  explicit A() { }
};

struct B {
  A a;
};

B b = { };

