#include <variant>

constexpr auto operator<=>(const auto& v, const auto& u) {
  return std::visit([&u](const auto& t) { return t <=> u; }, v);
}

constexpr std::variant<double, int> v{1};
static_assert(v < 2);

