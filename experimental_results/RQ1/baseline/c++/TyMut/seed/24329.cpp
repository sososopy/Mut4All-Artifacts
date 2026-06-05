
struct S {
    int x;
    S(int a) : x(a) {}
    S() = default;
};
void f() {
    S s;
}

