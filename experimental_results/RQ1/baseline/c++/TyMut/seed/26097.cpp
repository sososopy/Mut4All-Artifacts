
  template <template <typename> class... Args>
  struct Foo {
    static const int value = sizeof...(Args);
  };

  template <typename> struct Bar { };

  const int test = Foo<Bar>::value;

