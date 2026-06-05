
template <typename T> struct a
{
  int i;
  constexpr a (): i (b ()) { }
  constexpr int b () { return c (); }
  int c () const { throw 42; }
  constexpr operator bool () { return true; }
};

static_assert (a <int> (), "foo");
