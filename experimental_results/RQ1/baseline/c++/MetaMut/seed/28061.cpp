
  template<int> struct A {
    static const bool x;
    static_assert(&x);   // odr-uses A<...>::x
  };
  template<int I> const bool A<I>::x=(throw 0,false);

  void f(A<0>) {}        // A<0> must be complete, so is instantiated
  int main() {}

