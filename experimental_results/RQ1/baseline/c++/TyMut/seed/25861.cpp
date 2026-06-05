
void foo()
{
  x;
#pragma omp parallel for reduction(+:x)
  for (int i = 0; i < 10; ++i) ;
}
