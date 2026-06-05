
#include <functional>

struct A
{
  typedef bool X;
  template<int> X foo() const;
};

std::function<bool(const A*)> b(&A::foo<0>);
