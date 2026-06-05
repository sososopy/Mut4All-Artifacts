
struct X {
    X();
    X(X const&);
    X(X&&) = delete;
};
X make() {
    X a;
    return a;
}

