
#include <iostream>
#include <string>
#include <vector>

struct S
{
  int A;
  std::string B;
};

struct V
{
  std::vector<S> v;
};

static const V v{ { { { 1, "2" } } } };

int main()
{
  if (v.v[0].A != 1 || v.v[0].B[0] != '2')
    __builtin_abort();
  return 0;
}
