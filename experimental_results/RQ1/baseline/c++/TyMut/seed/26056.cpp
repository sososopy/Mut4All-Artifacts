

#include <iostream>

template<typename T> auto default_lambda = [](const T&) { return 42; };

template<typename T>
struct A {
  template<class U = decltype(default_lambda<T>)>
  void func(U u = default_lambda<T>) { std::cout << u(T{}); }
};

int main() {
  A<int> f;
  f.func();
}

