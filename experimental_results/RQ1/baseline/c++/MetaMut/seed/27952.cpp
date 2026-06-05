
#include <type_traits>
#include <vector>

struct A {
  template <typename>
  static std::false_type fn(...);
};

template <class T>
struct B : public A  {
  using fn = decltype(fn<T>(0));
  using type = typename std::conditional<fn::value, std::true_type, std::false_type>::type;
};

int main(){
  static_assert(B<std::vector<int>>::type::value, "assertion failed");
}
