
struct A {
 A();
 A(A&&);
};

struct B {
 A const a = A();
};

B b;
