
struct foo {
    explicit foo(int&&) {}
};

struct bar: private foo {
    using foo::foo;
};

int main()
{
    bar b { 42 };
}

