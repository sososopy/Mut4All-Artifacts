#include <functional>

template<class>
struct A {
  A(std::function<void()> v = [](){}) {}
};

template<class>
struct B {
  A<int> member;
  B() = default;
  B(int v) {}
};

int main() {
  B<int>();
  B<int>(0);
}


