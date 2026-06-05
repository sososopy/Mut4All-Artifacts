
#include <vector>
template<typename T> struct Foo {
  std::vector<int> member;
  Foo() {
    auto f = [this]() {
#if 0
      this->
#endif
      member.push_back(42);
    };
  };
};
void bar() { Foo<int> foo; }

