
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
  auto x = std::vector<int>({1, 2, 3});

  auto y = [x] () 
    mutable {
    for (auto &i: x)
      i ++;
    return x;
  };
  
  for (const auto &i: y())
    std::cout << i << std::endl;

  for (const auto &i: y())
    std::cout << i << std::endl;
  
  return 0;
}
