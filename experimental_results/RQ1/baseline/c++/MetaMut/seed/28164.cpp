
void foo(int *x);

void test1(int * const begin, const int len) {
#pragma omp parallel default(none)
#pragma omp for
for(int x = 0; x < len; x++)
    foo(begin + x);
}

