
#include <cassert>

class A {
        virtual void f(){};
    public:
        int x;
        A(int in): x(in) {};
};

class B: public A {
    public:
        int y;
        B(int in):A(in-1), y(in) {};
};

int test(void) {
    int res;
    B b(2);
    A* bp = &b;
    void* vp = dynamic_cast<void*>(bp);
    if (  ((A*)vp)->x == 1
       && ((B*)vp)->y == 2
       ){
        return 1; // PASS
    } else {
        return 0; // FAIL
    }
}

int main (void) {
    assert (test ());
}
