
struct S
{
  long a = 1;
};

template <typename T, unsigned long N>
struct A
{
  T value[N];
};

void *fn()
{
  return new A<A<A<S, 400>, 400>, 400>;
}
