
#include <iostream>
struct C{
   C()=delete;
   C(const C& other): x(other.x+1){}
   int x=10;
};
struct D{
   C c0{c0};  // << -- compiles without warning
 //C c1{c1};  // << -- correctly rejected
};
int main(){
   D d;
   std::cout << d.c0.x << std::endl; // prints 1
}
