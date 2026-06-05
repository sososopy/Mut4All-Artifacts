

namespace N {
    struct S {
        friend void f(S&) {}
    };
    namespace {
        int f;
    }
}

int main() {
    N::f = 42;
}

