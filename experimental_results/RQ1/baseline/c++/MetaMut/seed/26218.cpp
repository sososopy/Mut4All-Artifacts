
#include <iostream>

struct A
{
  static constexpr auto b{1.0};
};

constexpr decltype(A::b) A::b;

int main(int argc, char** argv)
{
  std::cout << &A::b << std::endl;
}
