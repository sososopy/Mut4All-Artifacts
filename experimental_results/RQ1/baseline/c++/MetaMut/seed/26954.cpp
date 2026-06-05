
#include <iostream>
#include <exception>

using namespace std;

class A {
public:
  A(int new_a) : a(new_a) { }

  ~A() noexcept(false) {
    cout << "a" << a <<".~A() ";

    if(std::uncaught_exception())
      cout << "Unwinding";

    cout << endl;

    if(a==4)
      throw a;
  } 

  int a;
};

int main()
{
  try {
    A a1(1), a2(2);
    A arr[] = {3,4,5};

  } catch(...) { }
}

