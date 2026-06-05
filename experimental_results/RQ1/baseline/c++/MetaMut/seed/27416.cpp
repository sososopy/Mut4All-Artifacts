
template <class T>
void foo(T argc) {}

template <typename T>
int tmain(T argc) {
  typedef double (*chunk_t)[argc[0][0]];
#pragma omp parallel
  {
  foo(argc);
  chunk_t var;(void)var[0][0];
  }
  return 0;
}

int global;
int main (int argc, char **argv) {
  int a[argc];
#pragma omp parallel shared(global, a) default(none)
  foo(a[1]), a[1] = global;


#pragma omp parallel private(global, a) default(none)
#pragma omp parallel shared(global, a) default(none)
  foo(a[1]), a[1] = global;


#pragma omp parallel shared(global, a) default(none)
#pragma omp parallel shared(global, a) default(none)
  foo(a[1]), a[1] = global;

  return tmain(argv);
}

