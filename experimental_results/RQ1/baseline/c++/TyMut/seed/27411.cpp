
struct X {
    int* x{new int{42}};
    X() = default;
    constexpr X(const X& x) : x{new int{*x.x}} { }
    constexpr ~X() { delete x; }
};
constexpr int f() { X x; return *X{x}.x; }
constexpr int z{f()};
