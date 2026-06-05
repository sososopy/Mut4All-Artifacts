
typedef unsigned long long int uint64_t;

template < typename T > static inline void
bar (T c)
{
  int a;
  register uint64_t b __asm__ ("r2") = (uint64_t)&a;
  __asm__ volatile (".long 0"::"d" (b):);
}

void
foo (uint64_t c)
{
  bar (c);
}

