
enum e_ : unsigned char { Z_, E_=sizeof(Z_) };
#include <stdio.h>
int main(void) {
  static_assert( E_ == 1, "E_ should be 1");
  printf("z is %d e is %d\n", Z_, E_ ); // prints 4
  printf("sizeof unsigned char is %d\n", sizeof(unsigned char)); // prints 1
  printf("sizeof e_ is %d\n", sizeof(e_)); // prints 1
  return 0;
}
