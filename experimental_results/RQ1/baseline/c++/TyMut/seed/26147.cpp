
struct A
{
  int foo()
  {
    [this]{ return foo; };
  }
};
