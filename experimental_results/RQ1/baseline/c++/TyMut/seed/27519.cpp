
template <typename>
void spam(decltype([]{}) *s)
{
  static_assert(__is_same(int, decltype(s)));
}

void foo()
{
  spam<int>(nullptr);
}
