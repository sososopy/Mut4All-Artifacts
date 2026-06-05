
#include <cstdio>
#include <iostream>

using namespace std;

struct Foo
{
    int a;
    int b;
};

struct Bar
{
    //Bar() = default;
    int d;
};

int main()
{
    Foo foo;
    Bar bar;

    printf("%d %d\n", foo.a, foo.b);

    return 0;
}

