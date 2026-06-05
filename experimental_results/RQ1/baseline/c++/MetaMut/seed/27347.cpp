
struct A {
    A(const A&) : x(7) {}

    template <typename U>
    A(U&&) : x(8) {}

    int x;
};

int foo() {
    A o1(true);
    A o2(o1);
    return o2.x;
}

struct B : A {
    using A::A;
};

int bar() {
    B o1(true);
    B o2(o1);
    return o2.x;
}
