
#include <concepts>
#include <iostream>

struct S {
    template<typename T> requires std::invocable<T>
    void f(T&& t);
};

template<typename T> requires std::invocable<T>
void S::f(T&& t) { t(); }

int main() {
    S{}.f([] { std::cerr << -1 << std::endl; });
}

