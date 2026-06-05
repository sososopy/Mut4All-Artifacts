
#include <concepts>
#include <cstddef>

template<class>
struct S {
  static constexpr auto f(const std::unsigned_integral auto value) -> std::size_t {
    return {};
  }
  static constexpr auto f(const std::signed_integral auto value) -> std::size_t {
    return f(static_cast<std::size_t>(value));
  }
};
