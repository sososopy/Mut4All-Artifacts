
void
vs (int *uj, int ct)
{
#pragma omp taskgroup task_reduction (* : uj[ct])
  ;
}
