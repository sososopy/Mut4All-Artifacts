
struct A
{
  struct B
  {
    void foo(auto);
  };

  void B::foo(auto) {}
};
