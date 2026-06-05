struct Foo {
  Foo() { x_ = 5; }
  Foo(const Foo&) { x_ = 5; }
  int x_;
};

class Super {
 public:
  Super(int i) : i_(i) {}
 private:
  int i_;
};

class Sub : public Super {
 public:
  Sub() : Super(f_.x_) {}  // Should warn here
 private:
  Foo f_;
};