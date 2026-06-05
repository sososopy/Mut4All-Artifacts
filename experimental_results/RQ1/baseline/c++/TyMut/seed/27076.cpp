
#include <memory>
using namespace std;

class A {
  const int a;
  const unique_ptr<A> b;
  const int c;
public:
  A(const A& copy) : a(copy.a), 
    b(copy.b), 
    c(copy.c) {
  }
};


