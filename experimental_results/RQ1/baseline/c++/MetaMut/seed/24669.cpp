
template<typename T> void foo(T) {}

template void foo(auto);

void bar()
{
  foo(0);
}
