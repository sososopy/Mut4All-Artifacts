
template <typename T>
int
foo(int b)
{
  static_assert(T::value, "toto");
  return b;
}

int
main()
{
  foo<int>(12);
}
