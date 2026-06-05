
#include <array>

struct X {
    X() {}
};

template<int N>
using C = std::array<X, N>;

void form(auto) {
  auto w = C<16>{};
}
