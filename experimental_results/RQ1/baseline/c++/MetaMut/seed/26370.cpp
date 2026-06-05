
#include <utility>

template<long... V>
void foo (std::integer_sequence<long, V...>)
{}

template<typename ...T>
struct U
{
  static constexpr long value = 1;
  constexpr operator int() = delete;
  constexpr operator long() { return value; }
};
template<typename T>
struct R
{
  using S = std::make_integer_sequence<long, U<T> {}>;
  R () noexcept(noexcept(foo (S ()))) {}
};

int
main()
{
  R<long>();
}

