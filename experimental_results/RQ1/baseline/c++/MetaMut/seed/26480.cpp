
struct A {
    int a0;
    int a1;
    int a2;

    friend inline constexpr bool operator<(const A& x, const A& y) {
        return std::tie(x.a0, x.a1, x.a2) < std::tie(y.a0, y.a1, y.a2);
    }
};
