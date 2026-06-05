

struct B {
    [[nodiscard]] virtual int f() = 0;
};

struct D : public B {
    [[nodiscard]] int f() override {return 1;}
};

int main() {
    B * b = new D;
    b->f();         // GNU C++ does not warn about discarded value
}

