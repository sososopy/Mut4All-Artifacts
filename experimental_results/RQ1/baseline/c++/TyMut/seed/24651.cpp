
    int f(int A, int B)
    {
      int r = 0;
      extern int *v;
      int a = 2;
      int b = 4;
      int n = 3;

      v[n] = 0;

    #pragma omp target map(to: v[a:b])
      r |= v[n];

    #pragma omp target map(to: v[A:B])
      r |= v[n];

      return r;
    }

