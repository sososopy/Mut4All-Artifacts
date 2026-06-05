
template <typename T, typename = decltype(T(0))>
void foo();

struct base {};
struct derived : base {};

void
bar()
{
  foo<derived>();
}
