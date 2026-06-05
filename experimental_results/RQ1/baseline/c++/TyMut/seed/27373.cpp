#include <compare>
#include <variant>

using variant = std::variant<int, double>;

constexpr auto operator<=>(const variant& v, const auto& u) {
  return std::visit([&u](const auto& t) -> std::partial_ordering { return t <=> u; }, v);
}

constexpr variant v{1};
static_assert(v < 2);

