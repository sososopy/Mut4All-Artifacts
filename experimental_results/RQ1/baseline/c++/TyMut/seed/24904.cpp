struct B {
    template <typename S>
    void foo();

    void bar();
};

template <typename T, typename S>
struct A : T {
    auto foo() {
        static_assert(requires { T::template foo<S>(); });  // fails with clang
        static_assert(requires { T::bar(); });  // works with clang and gcc 12.2
    }
};

int main() {
    A<B, double> a;
    a.foo();
}