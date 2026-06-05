
struct x {
  static void f() noexcept { }
  void (*_)() = [] { [&](auto) { f(); }(0); };
};
