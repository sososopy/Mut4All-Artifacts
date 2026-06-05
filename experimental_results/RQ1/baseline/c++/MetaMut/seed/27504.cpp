class A {
    int data = 9;
public:
    A()=default;
    A(int data) : data(data) {}
    void f() {if (data < 1) __builtin_abort(); }
};

A a[3]={1};

int main() {
    a[2].f();// Aborted
}
