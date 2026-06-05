

int a;

constexpr int *
foo ()
{
  return &a;
}

void blah (int *);

int
bar ()
{
  blah (foo ());
}

int
baz ()
{
  blah (foo ());
}
