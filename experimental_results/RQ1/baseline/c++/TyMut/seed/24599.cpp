
struct A
{
  int i;
};

template<typename... T, int A::* = &T::i> void foo() {}

template void foo<A>();
