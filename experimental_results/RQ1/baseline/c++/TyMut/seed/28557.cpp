#include <variant>

struct O {
  O() = default;
  O(O&&) = default;
  O(const O&) = default;
  O& operator=(const O&) = default;
  O& operator=(O&&) = default;
  ~O() = default;

  constexpr O(int x) {}
};

constexpr std::variant<float, O> v{42};

