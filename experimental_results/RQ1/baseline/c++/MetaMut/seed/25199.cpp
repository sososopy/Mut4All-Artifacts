
class b {
  struct B {
    B(int, int);
  } ah;

public:
  int c;
  b(const char *) : ah(0, c) {}
};
struct d {
  d(b);
};
struct e {
  e() {
    goto aj;
    if (0)
    aj:;
  }
};
struct f : d {
  f() : d("") { e(); }
} a;
