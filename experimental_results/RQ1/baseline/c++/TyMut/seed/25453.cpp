
#include <iostream>
#include <cstddef>

struct B {
    alignas(32) double x[1] = {0};
};

struct A
{
    A() { b1 = new B(); b2 = new B(); }

    B* b1;
    B* b2;
};

int main(int argc, char** argv) {
    A a;

    int ret = (ptrdiff_t) a.b1->x % 32 + (ptrdiff_t) a.b2->x % 32;

    std::cout << (ptrdiff_t) a.b1->x % 32 << "," << (ptrdiff_t) a.b2->x % 32 << "\n";

    return ret;
}
