
#include <cstdint>
#include <stdio.h>

const  int64_t kint64max  = ((int64_t)0x7FFFFFFFFFFFFFFFLL);

int main() {
  double d = (double)kint64max;
  int64_t v = d < kint64max ? d : kint64max;
  printf("%.20f -> %ld\n", d, v);
  return 0;
}
