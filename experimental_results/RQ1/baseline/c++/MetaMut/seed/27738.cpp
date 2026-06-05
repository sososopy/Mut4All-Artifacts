
template <class T> struct A {
    static void f();
    void f(int);

    void foo() {
        []{ f(); };
    }
};

void foo(A<int> &a) {
    a.foo();
}
