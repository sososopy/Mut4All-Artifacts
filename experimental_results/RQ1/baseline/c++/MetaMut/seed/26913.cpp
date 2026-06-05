
  struct a { constexpr int size() const { return 3; } };
  void f(a &r) { static_assert(r.size() == 3, "error"); }

