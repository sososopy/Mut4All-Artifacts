

int a;

template < int N, int I >
void fn1 ()
{
  const int x = I * a, y = x; 
  fn1 < y, I > ();
}

int 
main ()
{
  fn1 < 0, 0 > ();
  return 0;
}
