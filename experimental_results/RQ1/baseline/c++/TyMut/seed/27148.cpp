
#include <compare>

struct S {
  int a;
  bool operator<=>(const S&) const = default;
};

bool b = S{} < S{};
