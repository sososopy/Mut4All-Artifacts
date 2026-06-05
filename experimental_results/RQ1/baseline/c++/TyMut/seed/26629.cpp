
#include <iostream>

struct bar
{
  template<typename T> 
  auto operator()(T const& a) const -> decltype(~a) { return ~a; }
};

int main()
{  
  // works
  std::cout << ~1 << std::endl;

  // doesnt work
  bar z;  
  std::cout << z(1) << std::endl;
} 

