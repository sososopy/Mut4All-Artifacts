
#include <memory>

template <typename T>
struct A
{
  A (T*a) : a(a) {}
 
  A(A&&) = default;
  A& operator =(const A&) = default;

  std::shared_ptr<T> a;
};


#include "A.H"

template class A<int>;
int main()
{
  A<int> a = new int(19);
  A<int> b = std::move(a);
}


