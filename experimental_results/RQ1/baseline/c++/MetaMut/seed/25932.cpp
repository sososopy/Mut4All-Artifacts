
#include <iostream>
auto f();
auto f() { return 33; }
int main() {
   std::cout << f() << std::endl;
}
