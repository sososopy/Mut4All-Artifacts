
module;
#include <iostream>
#include <string_view>
export module Hello;

export void SayHello
  (std::string_view const &name)
{
  std::cout << "Hello " << name << "!\n";
}

/// main.cc
import Hello;

#include <string_view>

int main() {
  SayHello("world");
  return 0;
}
