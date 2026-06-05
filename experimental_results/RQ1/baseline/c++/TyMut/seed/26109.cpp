

#include <iostream>
using namespace std;

struct S { int a, b, c, d; }; //must be more than 8 bytes to generate bug

struct Base
{ int i;
  virtual S f() = 0;
  void g() { f(); }
};

struct Middle : virtual Base  //intermediate class needed to generate bug
{ int i;
};

struct Derived : Middle
{ S f()
  { cerr << this << endl
    return S();
  }
};

int main()
{ Derived d;
  d.f();
  d.g();  //incorrect value of this pointer in gcc 4.7.2
  d.f();
}
