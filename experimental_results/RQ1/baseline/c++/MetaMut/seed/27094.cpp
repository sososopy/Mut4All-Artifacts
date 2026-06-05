
#include <new>

struct S {
    int a [4];
} s;

void f (S *s) {
    char buf [sizeof s];
    S *t = new (buf) S (*s);

    // ...
}
