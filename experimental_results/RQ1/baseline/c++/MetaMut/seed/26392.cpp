
struct _Vector_base {
  ~_Vector_base();
};
int ColumnSmallestLastOrdering_OMP_i_MaxNumThreads,
    ColumnSmallestLastOrdering_OMP_i_MaxDegree;
void ColumnSmallestLastOrdering_OMP() {
#pragma omp for
  for (int i = 0; i < ColumnSmallestLastOrdering_OMP_i_MaxNumThreads; i++)
    new _Vector_base[ColumnSmallestLastOrdering_OMP_i_MaxDegree];
}
