
template <int a> struct b { static constexpr int c = a; };
template <typename d> auto aa() -> decltype(d());
template <int e> struct g {
  using f = __int128;
  constexpr operator f() { return e; }
};
template <class = void> struct h;
template <typename> struct k : b<8> {};
template <> struct h<> : k<char> {};
template <typename al> struct l {
  auto m() -> decltype(decltype(aa<al>() >> g<h<>::c - 1>{}){});
};
template <typename al, int, int> auto ao() -> decltype(l<al>());
template <int ar, int as> struct n { auto o() -> decltype(ao<char, ar, as>); };
template <int ar, int as = 2, class al> auto ap(al) -> decltype(n<ar, as>());
class G {
public:
  G();
  template <class al> constexpr G(al p) : ay(p) {}
  int ay;
};
template <class = int> using az = G;
auto i = az<>(g<7>{});
auto j = ap<2>(az<>{})
