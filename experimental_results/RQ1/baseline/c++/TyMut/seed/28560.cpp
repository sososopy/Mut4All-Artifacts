
#include <exception>

struct A : std::exception {
    A(int* q) : p(q) {}
    A(const A& a) : p(a.p) { *p = 12345; }
    A(A&& a) : p(a.p) { *p = 56789; }

    int* p;
};

int foo() {
    int i = 10;
    A a(&i);
    try {
        throw a;
    } catch (A&) {}
    return i;
}
