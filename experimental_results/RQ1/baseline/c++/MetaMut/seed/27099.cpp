

struct A {};
extern A a;

int i;
[[gnu::noinline, gnu::noclone]]
void f(A) { ++i; }

int main()
{
  f(a);
}
