

#include <type_traits>

template <class...>
struct all_same { static constexpr bool value = true; };
template <class T, class...Rest>
struct all_same<T, T, Rest...> : all_same<T, Rest...> {};
template <class T, class U, class...Rest>
struct all_same<T, U, Rest...> { static constexpr bool value = false; };

template <class...Us>
concept bool Same() {
  return all_same<Us...>::value;
}

template <class R>
using ValueType = int;

template <class I>
concept bool A() {
  return requires (I& i) {
    requires Same<ValueType<I>, ValueType<decltype(i++)>>();
  };
}

template <A>
  requires false
constexpr bool f() { return false; }
template <A>
constexpr bool f() { return true; }

int main() {
  static_assert(f<int>());
}

