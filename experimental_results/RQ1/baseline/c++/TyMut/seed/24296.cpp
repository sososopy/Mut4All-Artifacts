

#include <stdio.h>
#include <string>

int main() {
  std::string str("init");
  str == "bar";
  printf("%s\n", str.c_str());
  return 0;
}
