
template <typename a> class b {
public:
  void c(const a &);
};
class B {
  void d();
  b<const int *> e;
};
long f;
void B::d() { e.c((int *)f); }

