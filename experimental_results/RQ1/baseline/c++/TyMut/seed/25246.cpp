

#include <initializer_list>

struct A
{ 
  ~A () {}
};

void foo (A & v)
{ 
  for (A a : { v }) {};
}
