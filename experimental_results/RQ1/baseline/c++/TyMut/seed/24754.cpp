void f(int& x) {
    (void)x;
}

class C {
    int x;
    void g() __restrict;
};

void C::g() __restrict {
    f(this->x);
}