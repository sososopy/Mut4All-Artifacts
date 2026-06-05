
void f(auto x) { x(0); }
void g() {
  static constexpr auto h = [](...) { };
  f([](auto x) { h(x); });
}
