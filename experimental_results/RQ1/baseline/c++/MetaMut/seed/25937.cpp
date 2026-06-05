
#include <iostream>
#include <utility>

struct B { 
  // ...
};

template<typename ...T>
void operator++(B, T...) {
  std::cout << ((sizeof...(T) == 0) ? "prefix" : "postfix") << std::endl;
}

int main() {
  B b;
  b++;
  ++b;
}

