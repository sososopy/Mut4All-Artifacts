
template<typename T>
struct X {
  T a;
  int b;
};
template<typename T>
X(T, int) -> X<T>;  // <- does not work
// X(T) -> X<T>;  // < works

template<X V>
struct Y {};

template<typename T>
inline constexpr bool is_Y = false;
template<X NTTP>
inline constexpr bool is_Y<Y<NTTP>> = true;

X x(3, 3);
Y<X(3, 3)> y;
using my_y = Y<X(3, 3)>;
static_assert(is_Y<my_y>);
