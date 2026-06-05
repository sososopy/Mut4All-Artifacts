
template <typename a, int b> struct c {
  typedef a __attribute__((vector_size(b))) d;
};
template <typename a, int b> using e = typename c<a, b>::d;
template <typename a> using f = e<a, 4>;
struct g {
  g(f<int> h) : i{h[3]} {}
  g j() { return i; }
  f<int> i;
};
struct {
  g k();
} l;
class m {
  void n() const;
};
void m::n() const { l.k().j(); }

