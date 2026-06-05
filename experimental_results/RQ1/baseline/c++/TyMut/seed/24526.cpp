
#include <iostream>

struct Foo {
  int value;

  Foo() noexcept {
    std::cout << this << " constructed. Setting value to twelve.\n";
    value = 12;
  }
};

static thread_local Foo a{};
static thread_local Foo b;

static __attribute__((noinline)) void UseA() {
  const int value = a.value;
  std::cout << "Value of A: " << value << "\n";
}

static __attribute__((noinline)) void UseB() {
  const int value = b.value;
  std::cout << "Value of B: " << value << "\n";
}

int main(int argc, char** argv) {
  // std::cout << "Address of A: " << &a << "\n";
  // std::cout << "Address of B: " << &b << "\n";

  UseA();
  UseA();

  UseB();
  UseB();

  UseA();
  UseA();

  return 0;
}
