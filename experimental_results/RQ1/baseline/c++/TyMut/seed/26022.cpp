

int Foo;
int Bar;
void test () {
  auto L = []() {
    auto ret = Foo;
    return ret;
    return Bar;
  };
}
