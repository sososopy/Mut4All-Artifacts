
template <typename> void foo() {
  auto &a();
  __builtin_memset(a, 'X', 4);
}

