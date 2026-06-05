
#include <cmath>

double t1(double* x, int N) {
  double result = 0;
  int i;
#pragma simd reduction(max:result) private(i)
  for(int i=0; i<N; ++i) {
    result = std::fmax(x[i], result);
  }
  return result;
}
