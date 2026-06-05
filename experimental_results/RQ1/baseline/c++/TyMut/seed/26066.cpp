
struct Test {
  int method(int value) { return value; }

  template <typename T>
  auto test(T value) -> decltype(this->method(value));
};

template <typename T>
auto Test::test(T value) -> decltype(this->method(value)) {
  return this->method(value);
}

int main() {
  Test t;
  return t.test(0);
}
