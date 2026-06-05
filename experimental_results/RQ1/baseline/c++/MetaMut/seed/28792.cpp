

template <auto value>
struct Foo {
    using SomeTypeAlias = int;

    Foo()
    {}
};

template <class T>
struct Bar {
    T value;

    constexpr Bar(const T& value)
        : value{value}
    {}
};

template <int N>
struct Baz {};

constexpr auto baz = Baz<42>{};

const Foo<Bar<Baz<42>>{baz}> test{};


