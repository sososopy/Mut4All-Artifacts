
#include <initializer_list>

struct foo
{
    foo(std::initializer_list<int>) {}
};

struct bar
{
    explicit bar(std::initializer_list<int>) {}
    bar() {}
    void baz(foo) {}
    void baz(bar&&) {}
};

int main()
{
    bar x;
    x.baz({1,2});
}

