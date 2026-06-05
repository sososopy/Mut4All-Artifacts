
template <typename T, T... Values> struct A
{
  using type = int;
  template <type... Suffix> using array = A<type, Values..., Suffix...>;
  void
  m_fn1 ()
  {
    array<>::data;
  }
};
