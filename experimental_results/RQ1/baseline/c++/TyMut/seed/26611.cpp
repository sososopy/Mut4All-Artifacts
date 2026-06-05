
struct Bar
{
    Bar() = delete;
    Bar(Bar const &) = delete;
    Bar& operator=(Bar const&) = delete;
    Bar(Bar&&) = delete;
    Bar& operator=(Bar&&) = delete;

    explicit Bar(int value) : m_value(value) {}
    int m_value;
};

namespace A
{
    struct Foo : Bar{}; // this is the only difference.

    Bar MakeBar()
    {
        return Bar(10);
    }
}

namespace B
{
    struct Foo
    {
        Bar bar; // this is the only difference.
    };

    Bar MakeBar()
    {
        return Bar(10);
    }
}

int main()
{
    A::Foo foo{ A::MakeBar() };
    B::Foo bar{ B::MakeBar() };
}
