
template <typename a> class b {
public:
  b(a *);
  ~b();
};
class c {
  struct C {
    b<unsigned> d;
    float e;
  };
  c();
};
b<unsigned> f(b<char>);
c::c() { C a[]{f("")}; }
