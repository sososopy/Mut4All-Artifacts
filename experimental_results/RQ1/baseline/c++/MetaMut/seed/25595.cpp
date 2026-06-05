
struct A {
    int f() {return 0;}
} a;

struct B {
    template<int=0> struct C {
        int i = a.f();
    };
};
B::C<> c;
