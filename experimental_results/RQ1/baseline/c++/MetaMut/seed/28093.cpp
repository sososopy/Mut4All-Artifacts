
struct A {};

template <A a>
struct B {
    void f();
};

template <A a>
void B<a>::f() { }
