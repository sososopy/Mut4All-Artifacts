
  template <class T, int N>
  constexpr int foo () { return N; }

  struct B { unsigned c: foo<int>, 3(); };

