
struct foo {
    explicit foo(int) {}
};

template<typename T>
struct bar: T {
    using T::T;

    // Bad
    explicit bar(): T(0) {}

    void baz()
    {
        // Also bad
        using qux = T;
    }
};

int main()
{}

