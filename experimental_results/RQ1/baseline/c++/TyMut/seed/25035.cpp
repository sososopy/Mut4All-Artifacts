

struct X { };

namespace foo {
    template <class T>
    void bar() { T{} < T{}; }

    void operator<(const X&, const X&) {}
}

int main() {
    foo::bar<X>();
}

