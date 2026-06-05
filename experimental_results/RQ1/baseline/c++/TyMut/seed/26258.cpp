
struct A {};

struct B : virtual A, A {};

A foo(const B &b)
{
  return b;
}
