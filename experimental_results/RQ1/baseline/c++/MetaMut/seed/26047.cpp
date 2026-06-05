
#include <iostream>
#include <string>

struct bar {
  int value;
};

struct foo {
  foo( bar & b, int x) 
    {
    b.value = x;
    }
};
template<typename T>
struct baz {
  void operator()(bar & b, T argc) const
  {
  foo false_positive_unsused_variable(b,argc);
  std::cout << "Result is "<< b.value << std::endl;
  }
};

int main(int argc, char **argv) {
  bar b;
  baz<int> z;
  z(b, argc);
    
    return 0;
}
