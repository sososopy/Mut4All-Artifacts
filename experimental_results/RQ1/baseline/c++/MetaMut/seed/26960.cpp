

#include <iostream>

#define RETURN_TYPE auto

namespace {

struct S {
  RETURN_TYPE operator--() && {
    return 1;
  }
  
  RETURN_TYPE operator--() const && {
    return 2;
  }
   
  RETURN_TYPE operator--() & {
    return 3;
  }
  
  RETURN_TYPE operator--() const & {
    return 4;
  }
};

const S callme() { return {}; }

auto f1() {
  return --S();
}

auto f2() {
  return --callme();
}

auto f3() {
  S s;
  return --s;
}

auto f4() {
  const S s {};
  return --s;
}
  
}

int main() {
  std::cout << f1() << " ";
  std::cout << f2() << " ";
  std::cout << f3() << " ";
  std::cout << f4() << std::endl;
}

