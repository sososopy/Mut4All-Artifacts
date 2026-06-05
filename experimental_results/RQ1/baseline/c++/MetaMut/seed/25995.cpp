
struct Foo {
  Foo() {}
  Foo(const Foo&) {}
};
void f(...);
void g() {
  Foo foo;
  void (*fun)(...) = f;
  fun(foo);
  f(foo);
}

