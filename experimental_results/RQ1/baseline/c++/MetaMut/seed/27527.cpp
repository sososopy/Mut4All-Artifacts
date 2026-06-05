
struct A { A (int); A (char*); int i; };

int x;

A a1[] = {
  0L,
  &x,   // accurate location
  sizeof (A),
  __builtin_offsetof (A, i)
};
