
#include <iostream>
using std::cout;

struct A {
    int a;
    int& b;

    A(int x) : a(x), b(a) {}
    A(const A& other) : a(other.a), b(a) {}
    A() : a(0), b(a) {}
};

int foo(A a) {
    a.a *= a.b;
    return a.b;
}


int main() {
    A a(3);

    cout << foo(a) << '\n';
    return 0;
}
