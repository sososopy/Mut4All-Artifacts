
#include <iostream>
using namespace std;

struct S{
  int i = 123;
  S(int i) :i(i) {cout<<"S("<<i<<")"<<endl;}
  ~S() {cout<<"~S("<<i<<")"<<endl;}
};

struct T {
  T() {cout<<"T()"<<endl;}
  ~T() noexcept(false) {
    cout<<"~T()"<<endl;
    throw 0;
  }
};

S foo() {
  try {
    T t;
    return {3};
  } catch (...){}
  return {4};
}

int main() {
  foo();
}
