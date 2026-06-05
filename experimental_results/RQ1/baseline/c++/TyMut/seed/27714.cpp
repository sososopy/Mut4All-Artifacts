
template <typename T>
constexpr bool bar(T &)  { return true; }

template <typename T>
constexpr bool foo(T &x) { static_assert(bar(x),""); return bar(x); }

int main(int argc, char *argv[])
{
  int i;
  constexpr bool y = foo(i);
  return 0;
}
