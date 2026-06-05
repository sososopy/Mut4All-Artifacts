

struct Foo;

struct Bar
{
    using MemberT = int Foo::*;

    MemberT h_;

    constexpr Bar(MemberT h) : h_{h}
    {
    }
};

struct Foo
{
    int test = 0;

    static constexpr Bar bar {&Foo::test};
};

constexpr Bar Foo::bar;
