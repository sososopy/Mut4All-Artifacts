
#include <utility>
struct c
{
  static void f(
    std::pair<const double*, const int*> = std::pair<const double*, const int*>(0,0)
  ) {}
};

