
#include <iostream>
#include <type_traits>

class Foo {};

template <class T, class...>
using first_type = T;

template <class C>
class Map {};

template <class C>
struct traits {};

//#define FIX_GCC

#ifdef FIX_GCC
template <typename C>
struct traits<Map<first_type<C, std::enable_if_t<!std::is_const_v<C>>>>> {
  static constexpr int N = 2;
};
#else
template <typename C>
struct traits<Map<first_type<C>>> {
  static constexpr int N = 2;
};
#endif

template <typename C>
struct traits<Map<first_type<const C>>> {
  static constexpr int N = 3;
};

int main() {
  std::cout << traits<Map<Foo>>::N << std::endl;
  std::cout << traits<Map<const Foo>>::N << std::endl;

  return 0;
}
