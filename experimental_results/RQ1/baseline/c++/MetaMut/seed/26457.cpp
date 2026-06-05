
extern "C" void abort ();

constexpr const int *
foo ()
{
  static constexpr int a = 1;
  return &a;
}

consteval const int *
bar ()
{
  static constexpr int a = 1;
  return &a;
}

[[gnu::noipa]] void
baz (const int *x)
{
  if (*x != 1)
    abort ();
}

int
main ()
{
  constexpr const int *p = foo ();
  constexpr const int *q = bar ();
  baz (p);
  baz (q);
  if (p == q)
    abort ();
}
