
#include <iostream>

struct Foo {
    Foo() {}
    Foo(const Foo&) { std::cout << "Foo(const Foo&)\n"; }
};

struct A {
    A(Foo) {}
};

struct B : A {
    using A::A;
};

int main() {
    Foo f;
    B b(f);
}

