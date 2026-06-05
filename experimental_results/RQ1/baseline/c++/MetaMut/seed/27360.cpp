
#include <new>

struct Foo {
    Foo() = default;
    Foo(int _a) : a(_a) {}
    int a = 0;
};

void f()
{
    Foo arr[2];

    new (arr) Foo(10); // ok
    new (arr + 0) Foo(10); // ok
    new (&arr[0]) Foo(10); // ok

    new (&arr[1]) Foo(15); // ok
    new (arr + 1) Foo(15); // warning: placement new constructing an object of type 'Foo' and size '4' in a region of type 'Foo [2]' and size '0' [-Wplacement-new=]
}
