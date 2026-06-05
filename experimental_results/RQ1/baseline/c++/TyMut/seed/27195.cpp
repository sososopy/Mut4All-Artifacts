
template <typename a> class b {
  struct c {
    a h;
    constexpr c() {}
    ~c() {}
  } d;
};
struct e {
  int g;
  e() { g = 2; }
};
main() { static const b<e> f; }
