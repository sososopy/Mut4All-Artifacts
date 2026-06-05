
template <int a> struct b { static const int aa = a; };
struct c {
  static b<0> d();
};
template <bool, bool, class...> struct f;
template <class e, bool g, class... h> struct f<false, g, e, h...> : b<0> {};
struct i : f<decltype(c::d())::aa, b<0>::aa, int> {};
void j(long) {}
class k;
template <class, class = c> class l;
struct n {
  typedef b<0> m;
};
template <class> struct q {
  int r() { return ah; }
  int ah;
  q(int) {}
};
class s : q<int>, q<k> {
public:
  template <class an, class ao> s(an, ao) : q<int>(0), q<k>(ao()) {}
  int t() { return q<int>(*this).r(); }
};
template <class> class ar {
public:
  s at;
  ar() noexcept(i::aa);
  ~ar();
};
template <class e> ar<e>::ar() noexcept(i::aa) : at(nullptr, int()) {}
template <class e> ar<e>::~ar() {
  long o = at.t();
  j(o);
}
template <class, class> class l : ar<int> {
public:
  l() {}
  l &operator=(l &&) noexcept(b<n::m::aa>::aa);
};
template <class e, class az>
l<e, az> &l<e, az>::operator=(l &&) noexcept(b<n::m::aa>::aa) {
  return *this;
}
struct H {
  l<int> p;
};
class u {
  void v();
  H au;
};
void u::v() { au = {}; }
