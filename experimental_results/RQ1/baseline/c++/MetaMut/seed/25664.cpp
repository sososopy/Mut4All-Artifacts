
#include<iostream>

void test() {
  for(std::size_t n = 1; n < 4 ;++n ) 
    n >>=1; 
}

int main() {
  test();
}

