struct A {
    [[gnu::flatten]] A() {}
    [[gnu::flatten]] ~A() {}
};

A a;

