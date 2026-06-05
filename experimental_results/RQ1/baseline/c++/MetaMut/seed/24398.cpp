
struct A
{
  template<typename...T> A(T...) {}
};

struct B : A
{
  using A::A;
};

constexpr B b;
