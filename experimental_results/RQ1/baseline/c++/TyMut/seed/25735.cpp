

#include <iostream>

struct Elem
{
  static auto test()
  { return 12; }
};

int main()
{
  std::cout << Elem::test() << std::endl;
  return 0;
}

