
struct Foo
{
  Foo () __attribute__ ((nonnull(1)));
};

void
foo ()
{
  Foo ();
}
