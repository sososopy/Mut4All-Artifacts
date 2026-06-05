
void foo(int x) {
  [&]{
    switch (x) {
    case 0:
      foo(0);
      [[clang::fallthrough]];
    case 1:
      foo(1);
    }
  }();
}
