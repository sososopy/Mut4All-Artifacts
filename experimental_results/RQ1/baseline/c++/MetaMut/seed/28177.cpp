
struct a {
  int b;
  a(int) {
#pragma omp parallel firstprivate(b)
    --b;
  }
};

int c;
int main() { a d(c); }

