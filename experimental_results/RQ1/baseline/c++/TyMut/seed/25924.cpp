
#include <iostream>
struct A {
  A(){std::cerr<<"A()\n";}
  ~A(){std::cerr<<"~A()\n";}
  A(A&&){std::cerr<<"A(A&&)\n";}
  A(A const&){std::cerr<<"A(A const&)\n";}
};
A f(){
  alignas(32) A x;
  return x;
}
int main(){
  f();
}
