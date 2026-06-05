
enum omp_allocator_handle_t {
  omp_cgroup_mem_alloc = 6,
};

struct pd {
};

void
d9 ()
{
  int t7;
  pd gy[1] = { pd(), };

#pragma omp for reduction (& : gy) allocate (omp_cgroup_mem_alloc: gy)
  for (t7 = 0; t7 < 1; ++t7)
    {
    }
}

