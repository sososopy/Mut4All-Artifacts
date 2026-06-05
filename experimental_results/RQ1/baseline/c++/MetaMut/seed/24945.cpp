

#include <utility> // for std::move
struct A {
  int a; int b;
  
  A(A &&o) 
    : a(a), // I get a warning here...
      b(o.b) {}  
};
struct B { 
  int a; int b; 
  B(B &&o) 
    : a(std::move(a)),  // but sadly not here
      b(std::move(o.b)) {}
};

