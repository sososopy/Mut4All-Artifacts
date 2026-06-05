
#include<iostream>

void  f(size_t  const & x){
  std::cout <<  "f:x="  <<  x <<  std::endl;}

int main(void){
  size_t  x{1};
  size_t  y{2};
  size_t  z{4};
  size_t  const & i=std::min(z,x  + y);
  f(i);
  size_t  const & a=std::max(x,y  + z);
  f(a);}
