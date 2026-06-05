
#include <iostream>

struct A {
  template <class T>
  double operator()(T x) const {
    return 0;
  }
};

template <class X> concept bool C() {
  return requires {
    &X::operator();
  };
}

int main() {
  std::cout << C<A>() << '\n';
  return 0;
}
