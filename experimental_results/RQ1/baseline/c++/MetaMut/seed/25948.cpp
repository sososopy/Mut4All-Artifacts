
struct A { int i; };

void foo()
{
  A a;
  #pragma omp declare reduction (+: A: omp_out.i +: omp_in.i)
  #pragma omp parallel reduction (+: a)
  ;
}
