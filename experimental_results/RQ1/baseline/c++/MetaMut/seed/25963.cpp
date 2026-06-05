
struct A
{
  int i = [] { return decltype(i)(); }();
};
