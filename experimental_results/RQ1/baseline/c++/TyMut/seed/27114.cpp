
#include <iostream>

struct B {
    int n;
};

template <typename T>
struct A {
    B& get() const { return f; }

    B f;
};

int main() {
    A<int> a;
    a.f = {};

    a.get().n = 10;
    std::cout << a.f.n << std::endl;
    return 0;
}
