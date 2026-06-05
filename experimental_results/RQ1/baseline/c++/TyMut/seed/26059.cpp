
template<typename T, auto, int = 0> void foo(T, auto) {}

void bar()
{
  foo(0, 0);
}
