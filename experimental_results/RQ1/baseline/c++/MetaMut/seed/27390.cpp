
void
dh (int *vp, int pm, int ca)
{
#pragma omp depobj (pm) depend (iterator (ca = 0 : *vp), in: vp[ca])
    ;
}
