
struct S {
  S () {}
};

template <typename T>
struct U {
  T c[10];
  U () {
#pragma omp task affinity (iterator (i = 0 : 10 : 1): c[i])
    ;
  }
};

U<S> v;

