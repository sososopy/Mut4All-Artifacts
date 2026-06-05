
class A {};
struct predefined_macros {
  struct B {
    A (predefined_macros::*generator)();
  };
};
template <typename> class C {
  void m_fn1();
  predefined_macros predef;
};
template <typename ContextT> void C<ContextT>::m_fn1() {
predefined_macros::B m = (predef.*m.generator)();
}

