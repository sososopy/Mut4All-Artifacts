
typedef decltype(sizeof(1)) size_t;
struct udl_arg {
  const char *str;
  template <typename T> auto operator=(T &&value) const -> int {}
};
constexpr auto operator""_a(const char *s, size_t) -> udl_arg
{
  return {""};
}
template <typename T> void h(T &&);
template<int T>
int test(int a)
{
        h("t"_a="t");
}
auto t = test<1>(1);
