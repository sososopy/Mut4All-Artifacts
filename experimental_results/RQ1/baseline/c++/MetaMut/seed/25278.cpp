
template <typename F> void e(F) {}

template <int> void bar() {
  e([](const void) {});
}

void baz() { bar<1>; }
