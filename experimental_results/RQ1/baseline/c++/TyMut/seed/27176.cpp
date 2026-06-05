
template <typename T>
struct A {
    int a() {
        return 42;
    }
};

template <typename T>
struct B {
    int b(A<T> *p) {
        return p->template A<T>::a();
    }
};

int main() {
    A<int> a;
    B<int> b;
    return b.b(&a);
}
