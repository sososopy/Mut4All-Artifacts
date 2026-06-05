
struct A {
    explicit A(int);
};

struct B {
    A a;
};

A a{10};     // ok
B b{.a{10}}; // error: converting from initializer list
             // would use explicit constructor
