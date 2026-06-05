
template <class b> struct c {
  using d = b;
  template <class bb, class e> auto operator()(bb l, e) -> decltype(d{l});
};
template <class f, template <class> class g> h() {
  using i = g<f>;
  int a[]{};
  unsigned j;
  i{}(a, j);
}
k() { h<int *, c>(); }

