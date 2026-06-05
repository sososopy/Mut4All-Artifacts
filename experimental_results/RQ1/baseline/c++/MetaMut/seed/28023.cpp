
struct Foo {};
void test(const Foo&);
struct Bar {
  template <typename T> operator T&&();
};
int main() {
  test(Bar{});
}

