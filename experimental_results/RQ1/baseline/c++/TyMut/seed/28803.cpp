
struct X {
  struct A {
    constexpr ~A() noexcept(false) { }
  };

  constexpr A operator()(auto...) { return {}; }
};

void f() { []() consteval { X{}(); }(); }
