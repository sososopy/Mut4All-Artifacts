

const int& foo (const int& x) { return x; }

int
main ()
{
  int x = 1;
  const int& y = foo (x+1);
  return y != 2;
}
