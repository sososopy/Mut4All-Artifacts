
struct Foo {
  constexpr  Foo(int i) { p = new int(i); }
  constexpr ~Foo()      { delete p;       }

  constexpr bool compare(const Foo&) const { return true; }
  constexpr bool operator==(int y)   const { return compare(y); }
  int *p;
};

constexpr bool do_stuff()
{
  Foo f(0);
  bool b = f == 42;
  return b;
}

int main(int argc, char *argv[])
{
  static_assert(do_stuff());

  return do_stuff() ? 0 : -1;
}
