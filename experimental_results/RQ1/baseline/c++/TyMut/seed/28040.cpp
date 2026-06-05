
#include <type_traits>
#include <iostream>
#include <string>

#if !defined(SUPPRESS)
  template<typename O, typename = void>
  struct has_to_string : public std::false_type {};

  template<typename O>
  struct has_to_string<O, std::void_t<decltype(
      to_string(std::declval<O>()))>> : public std::true_type {};
#endif

  template<typename O, typename = void>
  struct has_std_to_string : public std::false_type {};

  template<typename O>
  struct has_std_to_string<O, std::void_t<decltype(
      std::to_string(std::declval<O>()))>> : public std::true_type {};

int main(int argc, char *argv[]) {
#if 0
  to_string(90);
  std::to_string(90);
#endif
  std::cout << "int : " << has_std_to_string<int>::value << std::endl;
}
