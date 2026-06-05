
template <typename T> struct foo {
  __attribute__((__always_inline__)) foo() {};
};
template <typename T> class bar {
  foo<void> alloc_ {};
};
template <typename... A>
void func1(A &&...) {
  bar<void>();
}
void func2() {
  func1();
}

