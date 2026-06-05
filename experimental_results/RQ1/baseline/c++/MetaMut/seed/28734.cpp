
#include <ranges>
#include <cassert>

struct NotAssignable {
  NotAssignable() = default;
  NotAssignable(const NotAssignable&) = default;
  NotAssignable(NotAssignable&&) = default;

  NotAssignable& operator=(const NotAssignable&) = delete;
  NotAssignable& operator=(NotAssignable&&) = delete;
};

constexpr bool test() {
  const std::ranges::single_view<NotAssignable> a;
  std::ranges::single_view<NotAssignable> b;
  b = a;
  b = std::move(a);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
