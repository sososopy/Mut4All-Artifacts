
struct S {
  ~S() {};
  typedef S S1;
};


void foo() {
  S y;
  typedef S S2;
  y.S1::~S2();
}

