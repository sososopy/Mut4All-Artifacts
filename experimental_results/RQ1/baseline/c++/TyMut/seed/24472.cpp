
#include <iostream>

template <class T>
struct Foo
{
  Foo()
  {}

  void foo()
  {std::cout << _foo << std::endl;}
  T _foo;
};

int main()
{
  Foo<double> f;
  f.foo();
  return 0;
}