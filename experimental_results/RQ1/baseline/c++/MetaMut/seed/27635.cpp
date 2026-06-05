
#include <type_traits>

int main()
{
  using T = int[2];
  T t(1, 2);

  static_assert(__is_constructible(T, int, int));
  static_assert(std::is_constructible_v<T, int, int>);
  static_assert(std::is_nothrow_constructible_v<T, int, int>);

  return t[0];
}

