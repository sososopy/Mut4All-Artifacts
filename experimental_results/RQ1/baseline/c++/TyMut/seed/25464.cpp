
template<class Derived>
struct Base {
    void A() { static_cast<Derived&>(*this).A(); }
    void B() { static_cast<Derived&>(*this).B(); }
};

struct Derived : Base<Derived> {
    void A() {}
    //void B() {}
};

int main() {
    Derived d;
    Base<Derived> &b = d;
    b.A();
    b.B();
}

