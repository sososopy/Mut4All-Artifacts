
#include <iostream>

struct A {
  void f () const { std::cerr << "const "; }
  void f ()       { std::cerr << "non-const "; }

  void g () const {
    [this] { f (); this->f (); } ();
  }
};

int main () {
  A {}.g ();
}


