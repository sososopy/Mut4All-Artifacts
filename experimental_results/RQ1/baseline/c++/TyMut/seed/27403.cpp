
typedef __Float32x4_t float32x4_t;
typedef float V [[gnu::vector_size(4 * sizeof (float))]];

template <typename T>
int
foo ()
{
  return 0;
}

int
bar ()
{
  return foo <float32x4_t> () + foo <__Float32x4_t> () + foo <V> () + foo <float [[gnu::vector_size(4 * sizeof (float))]]> ();
}
