
#include <cstdio>

class Foo {
public:

    /* Default version of foo.  */
    __attribute__ ((target("default")))
    int foo () { return 0; }

    /* SSSE3 version of foo.  */
    __attribute__ ((target("ssse3")))
    int foo () { return 0; }
};

int main () {
    Foo f;
    typedef int (Foo::* funcptr)();
    funcptr fp = &Foo::foo;

    j = (f.*fp)(); //< segfault here
    // f.foo(); //< works as expected
    
    printf ("all good\n");
    return 0;
}

