
#include <iostream>

struct A {
    int v = 0;
    A() {}
    A(const A &) : v(1) {}
};

int main() {
    A a({A{}});
    std::cout << a.v;  //this prints 0 in gcc instead of 1
}
