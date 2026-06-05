
struct A {
  virtual void member_fun() {}
};

struct B {
  A arr[1];
};

constexpr B func() {
  B b;
  return b;
}

void test() {
  func();
}
