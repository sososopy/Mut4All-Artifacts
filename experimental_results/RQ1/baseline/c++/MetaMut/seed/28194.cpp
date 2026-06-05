
template<typename T>
struct Bar {
    Bar() = default;
    Bar(double x) {}
};

struct Foo {
    template<typename T>
    operator T() {
        return T();
    }
    template<typename T>
    operator Bar<T>() {
        return Bar<T>();
    }
};

void test() {
    (void)static_cast<Bar<int>>(Foo());
}

