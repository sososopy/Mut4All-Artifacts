

struct A { virtual ~A(); };
struct B {
  B();
  B(B &&) = delete;
  A middle_field;
};
struct C {
  C() : b{{}, {}} {}
  B b[2];
};
