
template<typename T> void foo()
{
  #pragma omp parallel for
  for (typename T::X i = 0; i < 100; ++i)
    ;
}

void bar()
{
  foo<int>();
}
