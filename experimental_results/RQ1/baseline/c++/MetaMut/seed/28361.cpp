
template <typename T>
int foo(const T &t) {
  #pragma omp target parallel
  t();
  return 0;
}
struct S {
  int a = 15;
  int foo() {
    auto &&L = [&]() { return a; };
#pragma omp target
    L();
#pragma omp target parallel
    L();
    return a + ::foo(L);
  }
} s;
int main(int argc, char **argv) {
  int &b = argc;
  int &&c = 1;
  int *d = &argc;
  int a;
  auto &&L = [&]() { return argc + b + c + reinterpret_cast<long int>(d) + a; };
#pragma omp target firstprivate(argc) map(to : a)
  L();
#pragma omp target parallel
  L();
  return argc + s.foo();
}

