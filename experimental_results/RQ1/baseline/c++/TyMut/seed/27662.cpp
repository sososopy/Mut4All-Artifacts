
struct A {
  using btype = bool;
  template <typename T> A(T);
  btype data;
};
using boolean = A;
struct B {
  auto m_fn1() -> void;
  struct {
    boolean nmiValid = 0;
  } status;
};
auto B::m_fn1() -> void { status = {}; }

