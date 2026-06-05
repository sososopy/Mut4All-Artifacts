
struct A {
    int i;
    A(int ** iptrptr): i(1) {
        *iptrptr = &i;
    }
};

struct B {
    int* iptr;
    B(): iptr(0) {}
    ~B() {
        *iptr = 2;
    }
};

A foo() {
    B b;
    return A(&b.iptr);
}
