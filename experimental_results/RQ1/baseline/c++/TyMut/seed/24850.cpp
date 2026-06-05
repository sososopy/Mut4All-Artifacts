

struct S {};

constexpr int f(S s) {
    constexpr S _ = s;
    return 0;
}

S s;
constexpr int _ = f(s);



