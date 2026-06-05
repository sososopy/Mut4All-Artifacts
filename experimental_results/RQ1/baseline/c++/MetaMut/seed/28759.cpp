
template <typename T> void foo (T, T[], T *, int, int (& v)[], int *)
{
  #pragma omp parallel reduction (+:v) allocate(v)
  ;
}
void test ()
  {
  long x;
  long y[] {};
  long * z;
  int u;
  int v[4];
  int * w;
  foo (x, y, z, u, v, w);
}

