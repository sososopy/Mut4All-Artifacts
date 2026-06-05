
#define TEST(T) T
#define PARALLEL(X) TEST(X)
PARALLEL(
    for (int i = 0; i < N; i++) { \
      _Pragma("omp ordered") \
      S[0] += C[i] + D[i]; \
    })

