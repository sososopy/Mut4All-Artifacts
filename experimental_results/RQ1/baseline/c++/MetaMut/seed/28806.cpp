
template <int N = 1>
struct Foo
{
  template <typename ...Ts>
  Foo(Ts... xs) requires(sizeof...(xs)==N) {}
};

void bar()
{
  Foo x{64};
}
