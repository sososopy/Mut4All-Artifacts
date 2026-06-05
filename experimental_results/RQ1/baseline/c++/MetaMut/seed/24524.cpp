
    struct foo
    {
        unsigned value;

        foo();
        foo(unsigned value):
            value(value)
        {}
    };

    foo::foo()
        = default;

    int main()
    {
        foo bar{};
    }

