
struct vector {
    int *data;
    int n;
    constexpr vector() : data(new int[1]{}), n(1) {}
    constexpr ~vector() { delete [] data; }
};

struct Foo {
    constexpr auto foo() const {
        return vector();
    }
};

template <auto& f>
constexpr static auto bar() {
    [] { return f.foo().n; }();
    return true;
}

constexpr Foo foo;
constexpr auto dd = bar<foo>();
