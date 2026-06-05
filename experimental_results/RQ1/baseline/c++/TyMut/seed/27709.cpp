
struct S {
    S() = default;
    S(const S&) {}
};

S f() {
    const S m;
    return true ? m : throw 0;
}

int main() {}
