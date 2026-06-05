
struct A {
  int i;
};
struct B {
  A a{};
  constexpr B() : a() {}
  constexpr B(const B &rhs) : a(rhs.a) {}
};
struct C {
  B arr[1];
};

constexpr C
fn ()
{
  C c{};
  return c;
}

C c = fn();
