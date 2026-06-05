
#include <concepts>
#include <vector>

template <typename Ptr, std::regular_invocable<int> Fn>
struct foo_t {
  foo_t(Ptr ptr) {}
};

template <typename T>
using alias = foo_t<T, decltype([](int) { return 0; })>;

template <typename T>
auto fun(T const& t) {
  return alias<T>{t};
}

int main() {
  std::vector<int> v;
  auto const error = fun(v);
}
