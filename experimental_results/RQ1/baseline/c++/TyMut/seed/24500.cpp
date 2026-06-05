
struct A { };

template<class>
struct B {
    A a[1] = { };
};

int main () { B<void> b; }
