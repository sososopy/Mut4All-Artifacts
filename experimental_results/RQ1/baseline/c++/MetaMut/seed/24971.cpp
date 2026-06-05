
class A {
  template <typename> void As();
};
template <typename T> class B : A {
  void f() {
    A *g ;
    g ? g->As<T>() : nullptr;
  }
};
