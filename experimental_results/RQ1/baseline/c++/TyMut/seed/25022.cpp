

template <typename>
struct X
{
  template <unsigned int = 0>
  static constexpr int
  calc (void)
  {
    return 0;
  }

  static constexpr unsigned int value = calc ();     // <<<<<<<

  char foo[value];
};
