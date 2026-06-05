
struct A
{
  int i;
  constexpr A() {}
};

struct B
{
  A a;
};

constexpr B b[] = { {} };
