#include <iostream>

  //struct A {
   union A {
     int a = 3;
     int b;
   };
  //};

  int main() {
     A c{};
     std::cout << c.a << "\n";
  }

