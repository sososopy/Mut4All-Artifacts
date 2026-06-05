$ cat testcase.C
struct base {};
struct derived : base {};

template <typename T, typename R, typename = decltype(T(R()))>
void foo();

void
bar()
{
  foo<derived, base>();
}
