
struct A
{
  constexpr A(int, int = i) {}
  static const int i;
};

struct B : A
{
  using A::A;
};

constexpr B b(0);
