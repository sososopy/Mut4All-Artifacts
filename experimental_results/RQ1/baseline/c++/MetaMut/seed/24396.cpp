
namespace N { void f (); }

struct A {
  friend void N::f () { } // violates [class.friend]p6
};

int main () { }

