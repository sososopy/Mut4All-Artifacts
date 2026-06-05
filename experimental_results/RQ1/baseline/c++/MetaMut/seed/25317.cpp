

typedef int vec __attribute__ ((vector_size (sizeof (int))));
void foo (vec x, vec y, int z)
{
  x = (z == y);
}
