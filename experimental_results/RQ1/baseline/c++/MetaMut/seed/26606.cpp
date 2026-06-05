
#include <vector>

struct C {
  int a;
  constexpr C() : a(0) {}
  constexpr C(int a) : a(a) {}
};

static_assert( std::vector<C>({5}).size() == 5 );

