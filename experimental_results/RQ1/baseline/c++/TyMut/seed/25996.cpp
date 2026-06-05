
#include <iostream>
using namespace std;

template<class A, class B> bool Compare_GT(const A &a, const B &b) { if (a > b) return true; else return false; }

struct A {
   bool foo(int a) {
     Compare_GT(VARIABLE, a);
   }
   private:
     static const int VARIABLE;
};

const int A::VARIABLE = 30;

int main() {
   A *obj;
//   cout << "30>20 ? " << obj->foo(20) << "\n";
//   cout << "30>40 ? " << obj->foo(40) << "\n";
   return !obj->foo(20);
}
