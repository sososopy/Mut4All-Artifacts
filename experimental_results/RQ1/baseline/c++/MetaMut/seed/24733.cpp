
struct A
{
  int i;

  void foo()
  {
    [&](){ [&](int[i]){}; };
  }
};
