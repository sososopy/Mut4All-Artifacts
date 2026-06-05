#include <stdio.h>
#include <initializer_list>
struct S { S(int) { puts("S(int)"); } ~S() { puts("~S()"); } };
int main() {
  std::initializer_list<S> ss({42});  // note extra parentheses
  puts("main");
}

