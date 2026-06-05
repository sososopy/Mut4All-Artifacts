
typedef unsigned long long A;
typedef union
{
  struct B s;
  A a;
} U;
void f (A x, unsigned int b)
{
  const U y = {.a = x};
}

