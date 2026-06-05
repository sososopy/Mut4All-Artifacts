
struct A {
    struct B {
        int C = 0;
    };
    A() = default;
    A(int) {}
    std::pair<B, B> p;
};

