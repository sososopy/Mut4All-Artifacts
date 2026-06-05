
struct A {
    A();
    ~A();
};

struct B {
    A a{};
};
    
struct C {
    B array[100][100];
} c;
