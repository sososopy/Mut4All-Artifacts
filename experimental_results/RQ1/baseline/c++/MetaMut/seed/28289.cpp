
template<typename T>
struct A {
    void f();
    template<typename U>
    void f();
};

template<>
template<typename U>
void A<double>::f() { }
