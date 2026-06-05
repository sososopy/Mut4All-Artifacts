

struct A
{
  int a = 3;
};

struct B : public A
{};

struct C : public virtual B
{
  int c = a;
};

#include <iostream>

int main()
{
  C c;
  std::cout << c.a << " " << c.c << std::endl;
  return 0;
}

