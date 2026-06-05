#include <cstddef>
#include <initializer_list>

template <typename T, std::size_t>
class Foo {
 public:
  Foo(std::initializer_list<std::initializer_list<T>>) {}
};

// Note: Checking sizeof...(Ts) instead of sizeof...(init_lists) in the
// deduction guide prevents the crash.
template <typename... Ts>
Foo(std::initializer_list<Ts>... init_lists) -> Foo<int, sizeof...(init_lists)>;

int main() {
  Foo{{2}};
  return 0;
}