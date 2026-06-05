
union foo
{
    int a;
};

constexpr bool test()
{
    foo f{ .a = 42 };

    constexpr auto memPtr = &foo::a;

    return (f.*memPtr) == 42;
}

