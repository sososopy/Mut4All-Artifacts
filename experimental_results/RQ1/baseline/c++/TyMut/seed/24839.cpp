struct S {
  constexpr ~S();
};

void func(S const &a) {
  [a](auto b) {
    ^{
      (void)a;
    };
  }(12);
}