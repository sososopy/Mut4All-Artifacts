
struct A {};
struct E {};

struct R {
    operator E*();
};

struct S {
    operator E*();
};

struct B1  : R, S {
    operator A*();
};

void foo1(B1 b1, int E::* pmf) {
        int i = b1->*pmf;
}
