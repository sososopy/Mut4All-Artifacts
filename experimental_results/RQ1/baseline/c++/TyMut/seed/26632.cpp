

#include <algorithm>
#include <iostream>

struct A {
  A() { };
  A(const A&) { }
};

struct B {
  union {
    int m_1 = 0;
    int m_2;
  };
  A dummy;
};

int main()
{
  B b;
  b.m_1 = 1;
  B c(std::move(b));
  std::cout << c.m_1 << std::endl;
}


