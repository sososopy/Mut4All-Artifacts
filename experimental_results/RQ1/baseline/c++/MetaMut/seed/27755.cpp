
struct base {
    int i;
    void f(){}
};

template <void(base::*F)()>
struct derived : base {
    void g1() {
        return (this->*F)();
    }
    void g2() {
        base* p = this;
        return (p->*F)();
    }
};

void h() {
    derived<&base::f> x;
    x.g1();
    x.g2();
}
