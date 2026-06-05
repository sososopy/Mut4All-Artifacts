
struct A {
    int g = 0;

    A() {}
    A(const A&) {}
};

struct B : A {};

A u() { return A{}; }

int bug() { return B{u()}.g; }

int main() {
    return 0;
}

