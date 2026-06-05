
struct A {
  static void f();
};

struct B : A
{
  using A::f;         // #1
  static void f(int); // #2
  auto g() -> decltype(f());  // Call should be accepted via #1, instead rejected
};

