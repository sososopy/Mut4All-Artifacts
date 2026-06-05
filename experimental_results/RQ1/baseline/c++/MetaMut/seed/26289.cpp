
#include <iostream>

struct test_t
{
 test_t () { std::cout << " test_t @ " << this << std::endl; }
~test_t () { std::cout << "~test_t @ " << this << std::endl; throw 0; }
};

int main ()
{
 try
 {
  test_t const t; // (1)
 }
 catch (...)
 {
  std::cout << __LINE__ << std::endl; // (2)
 }
}
