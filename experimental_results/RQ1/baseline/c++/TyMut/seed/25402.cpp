
#include <iostream>

struct V {
  V(){std::cout << "V()\n";}
};

struct A : virtual V {
  A() : V{} {std::cout << "A()\n";}
};

struct B : A {
  B(): V{}, A{} {std::cout << "B()\n";}
};


int main(int argc, char **argv) {
  B b{};
}
