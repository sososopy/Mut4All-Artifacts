
void non_const_mem_ptr() {
  struct A {
  };
  constexpr A a = {1, 2};
  struct B {
    int A::*p;
    constexpr int g() const {
      return a.*p;
    };
  };
}
