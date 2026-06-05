
template <class T> void f(T);

struct A {
    friend void ::f(A);
};

