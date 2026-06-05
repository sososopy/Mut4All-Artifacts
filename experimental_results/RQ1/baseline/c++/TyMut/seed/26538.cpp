
struct Foo {
    constexpr Foo() = default;
    constexpr Foo(Foo const&) {}
};

struct Bar {
    Foo _;
};

template<Bar v = Bar{}>
struct Doppelganger
{
    void disguise() {
        [this]<typename = void>(){ this; }();
    }
};

void execute() {
    Doppelganger<>{}.disguise();
}

