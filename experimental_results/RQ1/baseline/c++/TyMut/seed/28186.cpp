
template <template <class> class a> struct b {
  template <class> static int c;
  decltype(c<a>);
};
template <typename> struct d {
  template <typename a, typename> bool e() const;
  template <typename, typename> bool e() const;
}
