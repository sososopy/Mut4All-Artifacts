
#include <utility>

template <typename T>
struct S {
  template <typename... Arg>
  S<T>(Arg&&... arg);
};

