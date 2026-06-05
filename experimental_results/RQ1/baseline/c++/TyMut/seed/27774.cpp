
  template<typename T>
  struct Foo
  {
    Foo (T) {}
    auto operator () (int) { return 0; }
  };

  // this compiles
  auto x = Foo(123)(0);

  // this doesn't
  auto y = (Foo(123))(0);

