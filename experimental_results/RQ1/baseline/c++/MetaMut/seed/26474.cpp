
struct Empty {};

class Foo {
public:
    constexpr Foo(int x, Empty y, int z) : a(x), b(y)
    {
        c = z;
    }

private:
    int a{};
    [[no_unique_address]] Empty b{};
    [[no_unique_address]] int c{};
};

constexpr Foo r{1, {}, 3};
