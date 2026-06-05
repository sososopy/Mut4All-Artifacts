
class a {
  struct b {
    int c;
    int f;
  };
  template <typename> void d();
  b e;
};
template <typename> void a::d() {
  int g;
  if (&g == &e.c)
    ;
  else if (&g == &e.f)
    ;
}

