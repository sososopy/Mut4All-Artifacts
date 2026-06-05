
struct A {
    A() noexcept {}
};

void foo() {
    using T = const A (&)[1];
    T{};
}
