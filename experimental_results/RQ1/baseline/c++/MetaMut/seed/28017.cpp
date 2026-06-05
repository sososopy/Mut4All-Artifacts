
void
f1 (void)
{
  X int b[2];
  b[0] = 1;
  #pragma omp target map(to: b)
  ;
}
