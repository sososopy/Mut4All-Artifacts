
struct foo {
  int bar();
};
typedef int bar_t();
void zed(bar_t foo::*) {
}
void baz() {
  zed(&foo::bar);
}
