
#include <initializer_list>

class X {
 public:
  // X(std::initializer_list<int> init);  // OK
  X(std::initializer_list<int> init = std::initializer_list<int>()){}
};

void f(){
  X x{1};
}
