
struct B {
protected:
    ~B() {}
};

struct C : B { int n; };

int f();

void g() {
    C c{{}, f()};
}

