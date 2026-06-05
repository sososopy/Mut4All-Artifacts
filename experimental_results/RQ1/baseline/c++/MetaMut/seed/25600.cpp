
struct Foo;

static void bar(Foo& foo)
{
    [&]() {
        foo;
    };
}

