
#include <initializer_list>
struct Foo
{
  Foo(std::initializer_list<int> = {}) {}
};
Foo f{1};
