
typedef long unsigned a;
template <bool, typename> struct b;
template <typename p> struct b<true, p> { typedef p c; };
template <bool d, typename p = void> using e = typename b<d, p>::c;
template <typename f, f...> struct g;
template <typename> struct h;
template <typename f, f... i> struct h<g<f, i...>> { using c = g<f, i...>; };
template <typename> struct j;
template <typename f, f... i> struct j<g<f, i...>> {
  using c = g<f, sizeof...(i)>;
};
template <typename f, f, typename = void> struct k;
template <typename f, f l> struct k<f, l, e<l == 0>> { using c = g<f>; };
template <typename f, f l> struct k<f, l, e<l && l % 2 == 0>> {
  using c = typename h<typename k<f, l / 2>::c>::c;
};
template <typename f, f l> struct k<f, l, e<l % 2>> {
  using c = typename j<typename k<f, l / 2>::c>::c;
};
template <typename f, f l> using m = typename k<f, l>::c;
template <typename> template <a... i> using n = g<unsigned long, i...>;
template <long l> using o = m<unsigned long, l>;
template <a i> void increasing(n<i>) {
  increasing(o<10>{});
}

