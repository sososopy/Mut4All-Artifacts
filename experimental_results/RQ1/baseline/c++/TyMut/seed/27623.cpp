
struct S {
    int a[5];
    bool operator==(const S&) const = default;
};

bool f(const S& x, const S& y) {
    return x == y;
}

