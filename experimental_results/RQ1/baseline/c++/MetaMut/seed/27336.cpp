
template <typename> using a = int;
template <bool, typename, typename> using b = int;
typedef char d;
template <long> using e = int;
template <int f, int q> struct h { using i = b<q, a<e<f>>, e<f>>; };
template <long f, bool g> using j = typename h<f, g>::i;
long ab, k, aj;
const d l[]{};
class m {
public:
  m(int);
};
class n {
  m ad() const;
  template <class ae> void o(long) const {
    using c __attribute__((aligned(1))) = const ae;
  }
  long p;
  template <class, class>
  auto s(unsigned long, unsigned long, unsigned long, unsigned long) const;
  template <bool = false> auto q(unsigned long, unsigned long) const;
};
template <class, class>
auto n::s(unsigned long, unsigned long, unsigned long, unsigned long t) const {
  o<d>(p);
  return t;
}
template <bool g> auto n::q(unsigned long p1, unsigned long p2) const {
  using r = j<4, false>;
  using ai = j<4, g>;
  return s<ai, r>(ab, k, p1, p2);
}
m n::ad() const {
  long f(l[aj]);
  m(q(8, f));
}

