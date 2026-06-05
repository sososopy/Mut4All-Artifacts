
struct A {};

template<typename T> void foo(T t)
{
  t.T::~X();
}

void bar()
{
  foo(A());
}
