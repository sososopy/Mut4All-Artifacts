
struct A {
        A(int i);
};

typedef A B;

struct C : B {
        using B::B; // compile error by gcc
};

struct D : B {
        using B::A; // this is compiled as inherited constructor
};

C c(0);
D d(0);
