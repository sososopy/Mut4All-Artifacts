
#include <omp.h>
#include <stdio.h>

#define INIT_BODY \
    int mem_size = 1 << 30; \
    \
    double start = omp_get_wtime(); \
    char*  data  = new char[mem_size]; \
    double end   = omp_get_wtime(); \
    \
    printf( "%f s for new char[%i].\n", end - start, mem_size ); \
    delete [] data;

template<typename T> void init1() { INIT_BODY }
void init2() { INIT_BODY }

int main(int argc, char* argv[])
{
    init1<int>(); // with template: takes about a second
    init2(); // without template: takes a few microseconds
}
