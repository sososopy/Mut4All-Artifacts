
template<typename T> struct A
{
  #pragma omp declare reduction (x : T : omp_out += omp_in + [](){ return 0; }()) initializer (omp_priv = 0)
};
