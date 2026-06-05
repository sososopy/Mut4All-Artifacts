
  template<class ...T> struct X {};
  template<class ...T> void foo(X<T, T...>... a);
  void test() { foo(X<int, int, double>(), X<double, int, double>()); }

