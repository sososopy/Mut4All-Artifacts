
struct X {
  virtual void f();
};
struct Z : X {};
constexpr X x = X(Z());
