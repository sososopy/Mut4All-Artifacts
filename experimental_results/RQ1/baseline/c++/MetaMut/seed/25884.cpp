
struct A
{
  static void foo();
};

template<typename> void bar()
{
  A().foo;
}
