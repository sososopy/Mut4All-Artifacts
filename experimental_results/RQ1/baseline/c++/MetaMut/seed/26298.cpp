
#include <iostream>
struct A {
   A() { std::cout << " A()" << std::endl; }
  ~A() { std::cout << "~A()" << std::endl; }
};
struct B {
  const A &a;
  ~B() { std::cout << "~B()" << std::endl; }
};
struct C {
  std::initializer_list<B> b;
  ~C() { std::cout << "~C()" << std::endl; }
};
int main() {
  const C &c = C{ { { A() }, { A() } } };
  std::cout << "-----" << std::endl;
}
