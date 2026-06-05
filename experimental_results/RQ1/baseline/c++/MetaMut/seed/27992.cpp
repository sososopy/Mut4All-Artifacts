
#include <stddef.h>

void* operator new(size_t, void* p) {
    return p;
}

int a();
void d(int p);

struct U {
    int p;

    U()
    : p(a()) {
    }

    ~U() {
        d(p);
    }
};

void f() {
    U lhs;
    new (&lhs) U;
}
