
constexpr int
foo (int x)
{
  int a;
  a = 5;
  return x + a;
}

static_assert (foo (2) == 7);

constexpr int
bar (int x)
{
  const int a; // { dg-error "" }
  constexpr int b; // { dg-error "" }
  return x;
}

constexpr int
baz (int x)
{
  int a;
  return x + a; // { dg-error "" }
}

constexpr int a = baz (5);

constexpr int
qux ()
{
  struct S { int a = -5; int b; } s;
  return s.a;
}

static_assert (qux () == -5);

constexpr int
quux ()
{
  struct S { int a = 9; int b; } s;
  return s.b; // { dg-error "" }
}

constexpr int b = quux ();

