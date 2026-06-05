
#include <iostream>
#include <tuple>

template <typename, typename> struct same_type;
template <typename T> struct same_type<T, T> {};

int main() {
  // std::tuple tuple(1, 'a', 2.3, true);
  auto tuple = std::make_tuple(1, 'a', 2.3, true);
  auto[i, c, d, b] = tuple;
  same_type<std::tuple_element<0, decltype(tuple)>::type, decltype(i)>{};
  same_type<decltype(i), int>{};
  same_type<decltype(c), char>{};
  same_type<decltype(d), double>{};
  same_type<decltype(b), bool>{};
  std::cout << std::boolalpha;
  std::cout << "i=" << i << " c=" << c << " d=" << d << " b=" << b << '\n';
}

