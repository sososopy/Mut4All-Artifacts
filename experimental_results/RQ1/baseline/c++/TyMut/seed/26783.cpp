
struct A {
    A() = default;
    A(const A&) = delete;
    A(A&&) = default;
};
    
A f() {
    A a;
    return true ? a : a;
}

