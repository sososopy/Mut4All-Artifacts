
struct A
{
  void foo(auto);
};

struct B
{
  friend void A::foo(auto);
};
