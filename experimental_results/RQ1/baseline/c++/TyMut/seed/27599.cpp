
struct S {
    S(S&&) = delete;
};

S foo();

struct A {
    [[no_unique_address]] S s = foo();
};

A a{};
