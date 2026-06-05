
struct A
{
  friend void foo(auto);
  friend void foo(auto) {}
};
