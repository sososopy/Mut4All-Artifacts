
constexpr auto
foo ()
{
  struct S { unsigned char d[1u << __CHAR_BIT__] = {}; } t;
  return t;
}

template <int N>
unsigned char
bar (unsigned char x)
{
  static constexpr auto t = foo ();
  return t.d[x];
}

