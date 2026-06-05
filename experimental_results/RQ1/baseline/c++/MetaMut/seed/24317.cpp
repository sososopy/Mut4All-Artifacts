
template<typename T> void foo(T)
{
  X;
  X;
}

void X();
void X(int);

void bar()
{
  foo(0);
}
