namespace detail {
  int _foo(int i) { return i; }
}

void bar(int i) {
  detail::foo(i);
}

