struct S {
  S() = default;
  S(const S&) = delete;
};

struct array {
  S s[2];
};

struct PS {
  constexpr array operator*() const { return {}; }
};

struct W {
  constexpr W(const PS& p)   // 1
    : t(*p) {}
  array t;
};

W w(PS{});

