
struct A {
    virtual void foo() {}
};

template<class T>
struct B : A {
    void foo() requires true {}
};

int main() {
    B<void> b;
}

