
using UI = unsigned int;
template<UI m, UI a>
struct mod{
  static UI calc(UI x) {
    UI res = a*x;
    if (m) res %=m;
    return res;
  }

};


#include<iostream>
int main() {
   std::cout << mod<3,2>::calc(7) << std::endl;
   std::cout << mod<0,2>::calc(7) << std::endl;

};
