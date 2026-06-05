struct S {
  void g() const [[clang::annotate_type("foo", "bar", 1)]] {}
};

void f() {
  auto l = [] () [[clang::annotate_type("foo", "bar", 1)]] {};
}