
struct A
{
  int i;
};

A a;

constexpr int* foo()
{
  return &a.i;
}

bool bar()
{
  return foo() == &a.i;
}
