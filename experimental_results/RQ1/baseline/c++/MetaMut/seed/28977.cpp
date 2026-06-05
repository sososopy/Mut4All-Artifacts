
struct foo {
  template<typename>
  friend struct bar;
};

struct bar {
  int baz;
};

bar var;
