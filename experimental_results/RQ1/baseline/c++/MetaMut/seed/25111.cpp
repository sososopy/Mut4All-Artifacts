

struct moveonly {
    moveonly(moveonly&&) = default;
    moveonly() = default;
};

struct A {
    A(moveonly) {}
};
struct B : A {
    using A::A;
};

B b(moveonly{});

