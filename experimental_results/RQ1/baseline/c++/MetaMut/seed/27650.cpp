#include <stdio.h>
#include <new>

class A {
    public:
    A() {};
    int m;
};


int main() {
    A* src = new A;
    src->m = 10;
    A* src_p = new(src) A;
    printf("%d\n", src->m);
}

