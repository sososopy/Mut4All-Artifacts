
struct S {
    virtual constexpr ~S() { }
};

consteval bool test() {
    const auto ptr = new S;
    delete ptr;

    return true;
}

static_assert(test());

