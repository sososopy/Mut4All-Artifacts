
#include <compare>

struct A{
      long i : 48;
      auto operator <=> (const A&) const = default;
      };

struct B{
      long i : 8;
      auto operator <=> (const B&) const = default;
      };

void f(A a, B b){
   a <=> a; //OK
   b <=> b; //error (see bellow)
   }

