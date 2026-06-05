
#include <initializer_list>

struct B {
  B(int) = delete;
  template<class T> B(std::initializer_list<T>);
};

int main() {
  B({0});
}
