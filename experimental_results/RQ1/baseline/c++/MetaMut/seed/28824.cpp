
//mod.cpp:
module;
#include <memory>
export module tree;
export template<typename T>
using p = std::shared_ptr<T>;



//main.cpp:
import tree;
int main() {
  p<long> t;
}

