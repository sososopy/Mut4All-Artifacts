
#include <concepts>

class A {
    template <std::floating_point... T>
    A(T&&... args) { }

    template <std::integral... T>
    A(T&&... args) { }
};

