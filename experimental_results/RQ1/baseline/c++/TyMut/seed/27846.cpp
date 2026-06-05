struct A {
  static constexpr int v {1};
};
struct B {
  static constexpr int v {1};
};
static_assert(&A::v == &A::v, "1");
static_assert(&A::v != &B::v, "2");

