
struct A { constexpr A(); };

void f() {
  A b;
}

constexpr A::A() {}


