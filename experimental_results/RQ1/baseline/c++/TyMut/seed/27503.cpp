
#include <array>
#include <algorithm>

constexpr auto f(auto algo) {
  return [=] {
    // this one is okay
    // algo(std::array{1, 0});
    // this one is also okay
    // auto it = algo(std::array{0, 1});
    auto it = algo(std::array{1, 0});
    return 0;
  }();
}

int main()
{
  static_assert(f(std::ranges::sort) == 0); 
}

