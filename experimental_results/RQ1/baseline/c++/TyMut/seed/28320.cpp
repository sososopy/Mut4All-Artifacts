struct A {
  template <typename D, typename T> void foo(D, T, int);
};
template <class, typename, class, class, class, class, class, class, class, class, class, class>
struct Z : A {
  static Z *z;
  void bar();
};
template <class T, typename B, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
Z<T, B, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> *Z<T, B, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>::z;
template <class T, typename B, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
void Z<T, B, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>::bar() {
  int a = 0, b = 1, c = 2;
  z->foo(a, b, c);
}
