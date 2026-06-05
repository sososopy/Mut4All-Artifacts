
struct A {
    consteval virtual int f() {
        return 1;
    }
};

struct B : A {
    consteval virtual int f() {
        return 0;
    }
};

consteval int f() {
    A* a = new B();
    return a->f();
}

int main() {
    return f();
}

