
#include <utility>

struct T { };

T f(const T& t)
{
  return std::move(t);
}

T g()
{
  return std::move(f());
}

