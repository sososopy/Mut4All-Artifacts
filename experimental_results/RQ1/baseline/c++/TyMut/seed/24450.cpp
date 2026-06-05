
#include <iostream>

int64_t foo()
{
  return 5;
}


int main( int, char** )
{
  int64_t biggerNum = 5000;
  const int64_t& minimum = std::min( foo(), biggerNum );

  std::cout << minimum << std::endl;

  return 0;
}
