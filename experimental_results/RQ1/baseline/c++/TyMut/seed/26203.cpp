
struct A
{
  template<int> struct B
  {
    #pragma omp declare reduction (x : int : omp_out |= omp_in)
  };
};

#pragma omp declare reduction (x : int : omp_out |= omp_in) initializer (omp_priv = 0)
