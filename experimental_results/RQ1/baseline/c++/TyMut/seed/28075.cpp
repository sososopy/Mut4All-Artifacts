
  // $ cat bug.ii
  class c {
    void d();
  };
  main() {
    c a;
  #pragma omp for
    for (auto b = a; b != a.d; b)
      ;
