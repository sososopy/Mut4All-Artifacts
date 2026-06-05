
void __assert_fail();
namespace A {
void g();
}
void bar() {
  using A::g;
  0 ? static_cast<g>(0) : __assert_fail;
}
