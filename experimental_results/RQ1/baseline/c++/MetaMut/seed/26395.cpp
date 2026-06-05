
struct A {
    int i = 0;

    A() {}
    A(const A&) { i = 1; }
    int g() { return i; }
};

struct B : A {};

A u() { return A{}; }

int main() {
    return B{u()}.g();
}

