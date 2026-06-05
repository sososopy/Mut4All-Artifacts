
template <typename a, typename b = a> b aa;
template <typename a> auto ab() -> decltype(aa<a>);
# 1 "" 3
template <typename br, typename c> concept d = __is_base_of(c, br);
template <typename> struct bz;
struct {
  template <typename cb, typename... cc>
  auto operator()(cb e, cc... g) -> decltype(e(g...));
} bw;
template <typename cd, typename... cc>
using ce = decltype(bw(ab<cd>(), ab<cc>...));
namespace ch {
struct cx;
}
template <typename cy, typename f> concept da = d<ce<cy, f>, ch::cx>;
template <typename cy, typename f> concept db = da<cy, f>;
struct {
  template <typename cd> bz<cd> operator()(cd);
} dc;
struct h {
  template <typename f, typename cy>
  requires db<cy, f> friend operator|(f, bz<cy>);
};
template <typename> struct bz : h {};
struct {
  template <typename cd> auto operator()(cd e) { return dc(e); }
} dm;
class {
  count() {
    count | dm([*this](auto) {})
  }
};

