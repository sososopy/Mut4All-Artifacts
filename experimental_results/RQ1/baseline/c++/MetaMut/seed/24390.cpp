
template<typename..., int> void foo();

void bar()
{
  foo<0>;
}
