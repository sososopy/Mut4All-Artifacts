
struct A {
        int * i;
        int & b() { return *i; }
};

int g = 42;

A a() {
        return A{ &g };
}

int main() {
        const int & i = a().b();
        return i;
}
