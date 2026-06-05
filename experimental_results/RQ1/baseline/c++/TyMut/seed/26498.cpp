struct B { static int f(); };
struct D : B { using B::f; };
void f(D d) { &d.f; }
