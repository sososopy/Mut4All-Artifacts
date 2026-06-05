
#include <initializer_list>

struct A {
  A(std::initializer_list<double>) {}
};

struct B : A {
  using A::A;
};

int main() {
  B b = {2.0};
  return 0;
}
