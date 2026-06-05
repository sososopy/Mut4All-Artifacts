
struct A {};

template<int> void foo(void (A::* f)())
{
  A a;
  &(a.*f);
}

template<int> void bar(void (A::* f)())
{
  A *p;
  &(p->*f);
}
