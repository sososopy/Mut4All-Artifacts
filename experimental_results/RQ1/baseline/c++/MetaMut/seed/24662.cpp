
template<typename T, T = T{}> struct A {};

template<typename T> void foo(A<T>) {}

void bar()
{
  foo(A<char>());
}
