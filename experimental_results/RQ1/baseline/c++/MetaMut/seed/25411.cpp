

#include <cstddef>
template <size_t N> struct Parser
{
  char arr[N][1];
  constexpr void setMember (const char *, int);
};
template <size_t N>
constexpr void
Parser<N>::setMember (const char *, int i)
{
  for (auto j = 0; j < 2; ++j)
    arr[i][j] = true;
}
template <typename... T> constexpr auto create_parser (T... a)
{
  const char *strings[]{a...};
  Parser<sizeof...(a)> p{};
  for (auto i = 0; i < sizeof...(a); ++i)
    p.setMember (strings[i], i);
  return p;
}
main () { constexpr auto a = create_parser ("", ""); }
