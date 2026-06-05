
#include <concepts>

template <std::integral auto> 
struct S {};

template <template <std::integral auto> class>
constexpr auto f() {}

int main() {
  f<S>();
}
