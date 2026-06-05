
template<typename T> bool foo(T)
{
  int i;
  static_assert(foo(i), "Error");
}
