
template<typename> struct A
{
  #pragma omp declare reduction (x : int : omp_out += omp_in) initializer (omp_priv = omp_priv)
};
