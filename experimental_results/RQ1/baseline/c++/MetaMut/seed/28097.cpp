
struct A {
    constexpr virtual int get() = 0;
};

struct B : A {
    constexpr int get() override {
        return 10;
    }
};

struct Foo {
    B b[1] = {};

    constexpr A * get_a() {
        // Seems to be the problem.
        return &(b[0]);
    }
};

constexpr int get() {
    Foo f;
    return f.get_a()->get();
}

constexpr auto a = get();

int main() {
    return a;
}
