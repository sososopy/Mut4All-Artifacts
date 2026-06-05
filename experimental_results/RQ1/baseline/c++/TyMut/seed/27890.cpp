
struct a;
struct b {
  static a c();
};
struct a : b {};
template <class> struct d {
  void e() { 0 ? b() : b::c(); }
};

