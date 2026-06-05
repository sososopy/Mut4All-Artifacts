
struct A
{
  int foo() const { return 0; }
};

template<typename> struct B
{
  A a;
  constexpr int bar() { return a.foo(); }
};

constexpr int i = B<void>().bar();
