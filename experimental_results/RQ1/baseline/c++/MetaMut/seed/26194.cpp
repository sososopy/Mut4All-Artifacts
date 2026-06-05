
struct Base {
    void foo() { }
    void bar() { }
};

struct Derived : protected Base {
    void foo() { this->Base::foo(); }
    using Base::bar;
};

int main() {
    Derived d;
    d.foo(); // OK
    d.bar(); // OK
    d.Derived::foo(); // OK
    d.Derived::bar(); // OK
    (d.*&Derived::foo)(); // OK
    (d.*&Derived::bar)(); // error
}
