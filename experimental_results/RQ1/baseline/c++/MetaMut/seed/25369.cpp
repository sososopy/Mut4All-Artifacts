

#include <functional>
#include <algorithm>
#include <vector>

using namespace std;
using namespace std::placeholders;
struct foo { 
  foo(){}
  void func(int) {}
};
int main() {
  vector<reference_wrapper<foo>> v;
  foo inst;
  reference_wrapper<foo> rw = inst;
  auto b1 = bind(&foo::func, rw, 0); // okay
  b1();
  auto b2 = bind(&foo::func, _1, 0); // bad
  b2(rw);
}

