struct B {
    B() {}
};

struct A {
    constexpr static inline B b2{};
    constinit static inline B b1{};
};

int main() {
    A a;
}

