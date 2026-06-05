#include <new>

struct b {
        virtual ~ b() {}
};

struct d : b {
        ~ d () {}
};

int main() {
        d o;
        o.~b();
        new( & o ) d;
}


