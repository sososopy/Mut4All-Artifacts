
struct A
{
  int foo();
};

template<typename> void bar()
{
  using (A().foo);
}
