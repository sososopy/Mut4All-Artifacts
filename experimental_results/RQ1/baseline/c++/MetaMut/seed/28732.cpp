

// typedef IndexType int;  // < works:
template <typename IndexType>  // < fails
void
foo (IndexType N, IndexType M)
{
  #pragma omp target teams distribute parallel for collapse(2)
  for (IndexType i = 0; i < N; ++i)
    for (IndexType k = i; k < M; ++k)
      ;
}
