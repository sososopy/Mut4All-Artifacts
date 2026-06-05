#include <iostream>
#include <functional>

int main(){
  class Test{
    public:
    std::function<void()> a{[]{ std::cout << "a\n"; }};
    std::function<void()> b{[]{ std::cout << "b\n"; }};
    std::function<void()> c{[]{ std::cout << "c\n"; }};
  };
  
  Test test{};
  test.a();
  test.b();
  test.c();
}

// output:
c
c
c

// expected:
a
b
c