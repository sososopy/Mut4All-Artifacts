
namespace a {
template <typename d, typename e> constexpr bool f = __is_same_as(d, e);
}
struct g {};
struct h;
template <typename i, typename j> auto operator+(i, j) {
  auto k = [](auto l) requires a::f<decltype(l), g>{};
  return k;
}
void m() {
  struct n {
  } b;
  n c;
  auto o = b + c;
  a::f<decltype(o(g{})), h>;
}
