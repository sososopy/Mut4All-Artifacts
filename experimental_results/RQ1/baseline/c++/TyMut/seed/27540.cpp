
int bar (const int &);

void
foo ()
{
#pragma omp simd
  for (int i = 0; i < bar (8); ++i)
    ;
}
