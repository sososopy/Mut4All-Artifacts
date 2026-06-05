
void bar (void)
{
#pragma omp parallel
  {
    int *p;
    *p = 345;
  }
}

