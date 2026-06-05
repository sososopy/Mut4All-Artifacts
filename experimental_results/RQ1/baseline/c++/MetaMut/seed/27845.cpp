#include <compare>

struct X
{
  friend std::strong_ordering operator<=>(X, X);
} x;

using T = decltype(x < x);

