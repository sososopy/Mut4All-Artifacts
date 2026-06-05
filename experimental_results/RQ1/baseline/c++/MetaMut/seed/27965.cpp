
template <int n>
void foo ()
{
  static int i {100};
  struct { int a {i++}; } b {};
}
int main ()
{
  foo<0> ();
}

