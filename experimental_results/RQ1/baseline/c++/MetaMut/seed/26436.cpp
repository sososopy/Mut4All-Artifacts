
void
foo ()
{
  [[omp::decl (declare simd)]]
  extern int bar (int);
  bar (0);
  [[omp::decl (threadprivate)]] static int a, b;
  static int c [[omp::decl (threadprivate)]];
  [[maybe_unused, omp::decl (threadprivate)]] static int d, e;
}

int f [[omp::decl (declare target, enter)]], g;
[[omp::decl (declare target)]] int h, i;
