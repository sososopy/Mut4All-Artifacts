
struct FooBase {
  constexpr virtual ~FooBase() {}
};

struct Foo : public FooBase {};

constexpr bool test()
{
  FooBase* pb = new Foo;
  delete pb;

  return true;
}

int main(int argc, char *argv[])
{
  static_assert(test());
  test(); // no problem

  return 0;
}

