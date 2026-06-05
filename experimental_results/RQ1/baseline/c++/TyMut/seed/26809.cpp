
#include <iostream>

struct test1 {
        alignas(int) alignas(double) char _; 
};

struct test2 {
        alignas(double) alignas(int) char _; 
};

int main()
{
  std::cout << "alignof(int) is " << alignof(int) << std::endl;
  std::cout << "alignof(double) is " << alignof(double) << std::endl;
  std::cout << "alignof(test1) is " << alignof(test1) << std::endl;
  std::cout << "alignof(test2) is " << alignof(test2) << std::endl;
}
