
struct A {
    virtual void foo() { }
};

struct B : A {
    void foo() override;
};

int main() {
    B b;
}
