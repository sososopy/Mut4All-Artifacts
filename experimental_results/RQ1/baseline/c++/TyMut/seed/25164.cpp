struct S { int a; };
#pragma omp declare simd linear(ref(a):1)              // { dg-bogus "applied to non-integral non-pointer variable" }
int foo (S &a);

