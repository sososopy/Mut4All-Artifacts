
void
foo (void)
{
  extern int var1;
  #pragma omp declare target to (var1)

  #pragma omp target
  var1++;
}

int
bar (int x)
{
  extern int var2;
  #pragma omp declare target to (var2)
  if (x)
    return var2;
  #pragma omp target
  var2++;
  return -1;
}
#pragma omp declare target to (bar)

#pragma omp declare target
int
baz (int x)
{
  extern int var3;
  if (x)
    return var3;
  #pragma omp target
  var3++;
  return -1;
}
#pragma omp end declare target
