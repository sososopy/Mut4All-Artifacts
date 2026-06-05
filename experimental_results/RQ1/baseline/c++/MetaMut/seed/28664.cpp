
#include <iostream>
#include <stdint.h>
#include <cstddef>
struct A {
  int a;
  uint64_t b;
  int c = -1;
};
struct B : public A {
    int d;
};
int main() {
    std::cout << "sizeof(A): " << sizeof(A);
    std::cout << ", sizeof(B): " << sizeof(B) << std::endl;
    std::cout << "offset of d in B: " << (int)offsetof(B, d) << std::endl; 
}
