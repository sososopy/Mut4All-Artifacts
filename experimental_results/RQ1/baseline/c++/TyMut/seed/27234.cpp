
template<typename T>
constexpr int
foo()
{
  static_assert(T(1) == 0);
  return 0;
}

constexpr int
bar(int a)
{
  return a;
}

static_assert(decltype(bar(foo<int>())){} == 0);
