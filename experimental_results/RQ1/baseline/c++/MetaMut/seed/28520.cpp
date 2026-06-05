
#pragma once

#include <iostream>

template<char const *s>
class Base
{
public:
  void print()
  {
    std::cout << s << std::endl;
  }
};



#pragma once

#include "base.hh"

constexpr char const hello[] = "Hello world!";

class Child : public Base<hello>
{
};




#include "child.hh"

int main(void)
{
  Child c;
  c.print();
  return 0;
}

