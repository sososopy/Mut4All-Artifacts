struct Foo {
    auto f() -> decltype(x); // error: use of undeclared identifier 'x'
    int x;
};

struct Bar {
    int x;
    auto f() -> decltype(x); // ok
};
