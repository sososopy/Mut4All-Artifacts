
struct A {
  const A* p = this;
};

struct E {
  A a = true ? A{} : A{};
};

constexpr E e{};

