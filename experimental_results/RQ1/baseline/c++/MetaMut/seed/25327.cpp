
constexpr void bar(int *b) {
  int i = 0;
  b[i++] = 1; // GCC failure here.
}

constexpr int foo() {
  int tmp[] = {0};
  bar(tmp);

  return tmp[0];
}

constexpr int cexprI = foo();

int main() {
  static_assert(cexprI, "");
  if (!foo())
    __builtin_abort();
}

