
#include <iostream>

template<typename T>
struct A {
  template<typename U> static auto default_lambda() {
    return [](const U&) { return 42; };
  }

  template<class U = decltype(default_lambda<T>())>
  void func(U u = default_lambda<T>()) { std::cout << u(T{}); }
};

int main() {
  A<int> f;
  f.func();
}

