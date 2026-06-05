
struct Outer {

  void Bar();

  struct Foo {
    void (Outer::*ptr)() ;
  };

  static constexpr Foo foo = { &Outer::Bar };
};
