
#include <cassert>

struct X {
    short x0 : 7;
    short x1 : 8;

    X() { init(); assert(get() == 3); }

    void init() { x0 = 1; x1 = 2; }

    int get() { return x0 + x1; }
};

struct S {
    [[no_unique_address]] X x;
    char c = 0;
};

int main() {
    S s;
    assert(s.x.get() == 3);
    return 0;
}
