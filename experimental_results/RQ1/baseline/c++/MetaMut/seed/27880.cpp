
struct X {
    int const& var;
};

void foo() {
    constexpr int i = 42;
    constexpr X x{i};
}
