
struct A {
  A();
  operator int();
};
template <int N> struct array {
  A elts[N];
  A *begin();
  A *end();
};
void fn() {
  for (int i : array<4>{})
    ;
}
