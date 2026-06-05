
typedef enum { a } b;
using c = b;
float d, e;
template <c> struct f {
  typedef float g;
  void h() const;
  template <typename i>
  void j(const int &, g *, float *, float *, const i *, const float *) const;
};
using k = f<a>;
template <>
template <typename i>
void k::j(const int &, g *, float *, float *, const i *, const float *) const;
template <>
template <typename i>
void k::j(const int &, g *, float *, float *, const i *, const float *) const {
  int l([&] { l; });
}
template <c m> void f<m>::h() const {
  int n;
  g o;
  j(n, &o, &d, &e, static_cast<char *>(0), 0);
}
template struct f<a>;

