
#include <experimental/type_traits>
#include <iostream>
#include <vector>
template <class T>
using formatted_input_t =
    decltype(std::declval<std::ostream>() << std::declval<T>());
template <class T>
bool is_ostreamable_v =
    std::experimental::is_detected<formatted_input_t, T>::value;
template <class T> constexpr bool is_ostreamable() {
  return std::experimental::is_detected<formatted_input_t, T>::value;
}
template <class T, std::enable_if_t<is_ostreamable<T>()> = true>
std::ostream operator<<(std::ostream, T);
int main() {
  using T = std::vector<int>;
  is_ostreamable_v<T>;
}
