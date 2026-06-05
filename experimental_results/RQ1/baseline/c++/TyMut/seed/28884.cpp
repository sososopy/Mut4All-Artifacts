
template <int a> struct b { static constexpr int c = a; };
template <int> struct h { typedef int d; };
template <bool e, typename> using f = typename h<e>::d;
namespace g {
  template <typename...> using i = b<false>;
  using j = long;
}
using g::i;
using g::j;
template <typename> using k = i<>;
template <typename... l> f<!i<l...>::c, j> m();
template <typename... n> f<!i<k<n>...>::c, j> m();

