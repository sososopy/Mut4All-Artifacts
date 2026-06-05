
struct X{
  template <unsigned long N> X(const char (&src)[N]) {}
  template <unsigned long N> X(char (&src)[N]) {}
};

X f() {
  char buf[1];
  return buf;
}
