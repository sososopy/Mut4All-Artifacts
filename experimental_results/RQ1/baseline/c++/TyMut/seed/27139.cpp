
#include <compare>

void foo() {}
void bar() {}

int main()
{
  auto p1 = &foo;
  auto p2 = &bar;
  return (p1 <=> p2) != 0 ? 0 : 1;
}
