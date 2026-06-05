
int *cq;

void
l0 (int va)
{
#pragma omp target update to (va, cq[:])
}

