
#include <type_traits>

template<class T>
struct A {
  template <class S>
    requires std::is_same<S, A>::value
  friend int f(const S& s) {
    return 0;
  }
};

template <class T>
struct B {
  template <class S>
    requires std::is_same<S, B>::value
  friend int f(const S& s) {
    return 1;
  }
};

int main() {
  A<int> a; B<int> b;
  int x = f(a);
  int y = f(b);
  return 0;
}
