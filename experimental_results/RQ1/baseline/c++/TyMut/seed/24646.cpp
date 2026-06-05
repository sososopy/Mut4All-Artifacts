
template<typename T> T foo(T);

void bar()
{
  foo((auto*)0);
}
