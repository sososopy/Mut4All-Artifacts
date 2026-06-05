
#include <map>

typedef int vec256_t __attribute__ ((__vector_size__ (32)));
std::map<int,vec256_t> m;

int main() {
  vec256_t v;
  m[0]=v;
  return 0;
}

