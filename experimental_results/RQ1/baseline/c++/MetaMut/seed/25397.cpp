
template <int N>
int f (int n)
{
  int x[1][n];

  return [x[0]] /* () { return 0; } */; 
}

int i = f<1>(1);
