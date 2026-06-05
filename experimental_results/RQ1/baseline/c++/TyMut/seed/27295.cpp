
#include <iostream>

struct foo {
    int content;
    foo(int x) : content { x } {}

  // works: auto operator <=> (foo const& rhs) const = default;
  // works: auto operator ==  (foo const& rhs) const { return content ==  rhs.content; }
  // fails: 
            auto operator <=> (foo const& rhs) const { return content <=> rhs.content; }
};

int main() {
  foo a { 1 };
  foo b { 2 };
  foo c { 1 };

  if (a != b) std::cout << "not ";
  std::cout  << "equal" << std::endl;


  if (!(a == c)) std::cout << "not ";
  std::cout  << "equal" << std::endl;

  return 0;

