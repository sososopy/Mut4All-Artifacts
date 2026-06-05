
#include <utility>
 
template<class T>
using decay = typename std::decay<T>::type;

template<template<class...>class T, class...X>
struct Part {
  template<class...Y>
  using type = T<X..., Y...>;
};
 
template<template<class...>class T, template<class>class U>
struct UCompose {
  // Not ok!
  template<class X, class...Y>
  using type = T<U<X>, Y...>;
 
  //// Ok!
  //template<class X>
  //using type = T<U<X>>;
};
 
int main() {
  using IsInt = Part<std::is_same, int>;
  static_assert(IsInt::type<int>::value, "");    // Ok!
  using IsIntD = UCompose<IsInt::type, decay>;
  static_assert(IsIntD::type<int&>::value, "");  // Not ok.
}


