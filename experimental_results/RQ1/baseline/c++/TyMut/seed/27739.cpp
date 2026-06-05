constexpr int
foo ()
{
  const int i = 5;
  const_cast<int &>(i) = 10;
  return i;
}

int
main ()
{
  int i = foo ();
}
