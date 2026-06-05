
template <typename a> constexpr a b(a c) { return c; }
template <typename m, typename d> struct e {
  m f;
  d g;
  constexpr e(m c, d h) : f(c), g(h) {}
};
template <typename m, typename d> constexpr e<m, d> i(m &&c, d h) {
  return e<m, d>(c, b(h));
}
typedef enum { j } k;
class l {
  e<k, int> m_fn1() const;
};
void n() {
  int a;
  i(a, "");
}
e<k, int> l::m_fn1() const { i(j, ""); }

