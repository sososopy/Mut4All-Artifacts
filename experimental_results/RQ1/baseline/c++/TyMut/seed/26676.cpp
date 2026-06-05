
template <class T>
struct A {
    int n;
    T a[];
};

struct B { B (int); };

A<B> ab;
