
unsigned char a[1];
struct b {};
struct c : b {
  c(b);
  c(b &&);
};
struct B {
  operator c();
  static B d(unsigned char *, short, short, bool);
};
struct e {
  unsigned short f(bool) const;
};
#pragma GCC diagnostic error "-Wconversion"
short g, h;
unsigned short e::f(bool i) const {
  c(B::d(a, g, h, i));
  return 0;
}

