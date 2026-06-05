

#include <initializer_list>

void f(int) { }

void g()
{
  for (auto i : { 1, 2, 3 })
    f (i);
  f(42);
}

int main()
{
  g();
  g();
}
