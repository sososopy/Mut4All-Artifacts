
struct Type {
  template <int i>
  static void Foo() {}
  template <>
  void Foo<0>() {}
};

void call() { Type::Foo<0>(); }
