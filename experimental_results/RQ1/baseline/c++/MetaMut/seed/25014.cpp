
template <int> struct A { int i; };
struct B { int i; };
struct C {
    static const A<0> a = { 0 };
    static const B b = { 1 };
};
