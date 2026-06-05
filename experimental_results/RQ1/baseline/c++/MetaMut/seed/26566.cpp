

struct B {
  // implicitly declared default ctor
  A a;
};
struct C {
  C();
  A a;
};
C::C() { }

