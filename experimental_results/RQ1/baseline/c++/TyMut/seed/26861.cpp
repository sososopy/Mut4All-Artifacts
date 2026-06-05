
#include <new>

struct A {
    char a [4];
} a;

void foo ()
{
    new (&a.a [5]) char;
}
