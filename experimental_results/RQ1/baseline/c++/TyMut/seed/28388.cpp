
struct S {
  static void operator() (int);
  static void baz (int);
};
S &foo (int);

void
bar ()
{
  foo (0) (0);
  foo (1).baz (1);
}
