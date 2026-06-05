
struct Base { void Foo() {} };
struct Derived : private Base {};
void f(Derived *p, void (Base::*f)()) { (p->*f)(); }
