
struct Foo {
  Foo() {}
  Foo(const Foo&) {}
};

void f(...);

void g() {
  Foo foo;
  f(foo);
  void (*fp)(...) = f;
  fp(foo);
}
