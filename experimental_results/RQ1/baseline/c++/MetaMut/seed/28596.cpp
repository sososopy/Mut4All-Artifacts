
#include <cstdint>

struct Foo {
    int8_t a, b;

    Foo(): a(0), b(1) {}
};

struct Bar {
    Bar();

    Foo x0;
    Foo x1;
    Foo x2;
    Foo x3;
};


void foo(Foo&);

Bar::Bar() { foo(x0); }
