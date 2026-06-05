struct S {
  int x;

  auto foo() {
    return [*this](this auto&&) {
      __builtin_printf("%d ", x);
      x = 10;
    };
  }
};

int main() {
  S s{ 5 };
  const auto l = s.foo();
  l();
  s.x = 15;
  l();
  __builtin_printf("%d\n", s.x);
}