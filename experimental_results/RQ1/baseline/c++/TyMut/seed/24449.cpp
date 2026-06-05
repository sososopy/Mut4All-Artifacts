class B {
public:
    double x[2];
};
class A {
    B b;
public:
    B getB(void) { return b; }
};

double foo(A a) {
    double * x = &(a.getB().x[0]);
    return x[0];
}

