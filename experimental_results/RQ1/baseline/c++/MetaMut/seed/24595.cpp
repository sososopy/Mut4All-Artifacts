
template <int& F>
struct Foo {
  typedef int Bar;

  static Bar cache;
};

// template <int& F> int Foo<F>::cache;  // OK

template <int& F> typename Foo<F>::Bar Foo<F>::cache;

