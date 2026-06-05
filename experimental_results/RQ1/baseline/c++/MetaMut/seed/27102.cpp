
template <typename> void fn1() {
  [](auto) {
    enum { VALUE };
    VALUE;
  };
}
main() { fn1<void>; }
