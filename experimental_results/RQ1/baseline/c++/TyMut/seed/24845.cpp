struct B {
    int i;
};

int foo() {
    auto [x] = B{1};
    [x]() {
        x = 2;
    }();
    return x;
}