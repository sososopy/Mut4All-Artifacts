
template < typename... TYPES >
struct A {
    template < TYPES... types >
    void Get() { }
};

void f() {}

int main() {
    A<decltype(&f)> a;
    a.Get<&f>();
}

