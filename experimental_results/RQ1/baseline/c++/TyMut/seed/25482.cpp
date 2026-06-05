

#include <string>
#include <iostream>
template <typename T>
struct has {
    template <typename>
    constexpr static int test(...) {
      return 0;
    }
    template <typename C>
    constexpr static int test(decltype(sizeof(C::x))) {  // Doesn't compile.
      return 1;   // Is a member variable.
    }
    template <typename C, int c =
        sizeof(decltype(((C*)nullptr)->x()))>
    constexpr static int test(int) {
      return 2;   // Is a member function.
    }
    static const int value = test<T>(0);
};
struct foo {
    int x();
};
struct bar {
    int x;
};
int main() {
    std::cout << has<int>::value << std::endl;
    std::cout << has<foo>::value << std::endl;
    std::cout << has<bar>::value << std::endl;
}
