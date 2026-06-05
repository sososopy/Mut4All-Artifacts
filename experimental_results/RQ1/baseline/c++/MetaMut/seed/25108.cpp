

struct S {
    constexpr int operator=(int) const { return 1; }
};
constexpr S x;
struct S1 {
    int f : x = 3;
};
