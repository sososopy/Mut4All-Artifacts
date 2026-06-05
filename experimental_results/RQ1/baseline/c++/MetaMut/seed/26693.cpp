
#include <array>

constexpr void increment(double& x) {
  ++x;
}

constexpr std::array<double, 3> f(int a, int b, int c) {
  std::array<double, 3> r = {};
  std::get<0>(r) = a;
  std::get<1>(r) = b;
  std::get<2>(r) = c;
  increment(std::get<2>(r));
  return r;
}

int main() {
  return 0;
}
