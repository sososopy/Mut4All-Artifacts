struct A { virtual void a(); };

struct B : A {
  constexpr B(int);
  constexpr ~B() { }
};

struct D : B {
  constexpr D() : B(42) { }
};

constexpr D d;
