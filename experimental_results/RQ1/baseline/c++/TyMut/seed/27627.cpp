
void f() {
  extern void g();
  [] (auto) { g(); }(0);
}
