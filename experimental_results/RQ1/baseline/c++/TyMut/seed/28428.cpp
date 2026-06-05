struct A {
  const A *p = this;
};

constexpr A
bar (A)
{
  return {};
}

constexpr A a = bar (A{});
constexpr auto *p = a.p; // #1

