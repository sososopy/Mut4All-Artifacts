
struct S { int a, b; };

constexpr S f ()
{
  int x = 0;
  S z = { 0, ++x }; 
  return z;
}

constexpr S s = f ();

static_assert (s.a + 1 == s.b, "");
