
void foo(float *f, double *r) {
#pragma acc kernels loop create(f) copy(r)
for(;;) {}
}

