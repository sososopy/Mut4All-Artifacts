
struct X {
    explicit X() { }
};

struct Aggr {
    X x;
};

Aggr f() {
    return Aggr{.x{}};
}

