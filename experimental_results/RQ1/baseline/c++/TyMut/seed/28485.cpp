
#include <array>
struct P {
  constexpr P(int i) : e{i} {}
  auto operator<=>(P const &) const = default;
  std::array<int, 1> e;
};
constexpr auto operator-(P lhs, P) { return lhs; }
constexpr P p{1}, q{0};
static_assert(p - q != 0);

