
struct A {
    int i;
    constexpr A(int n) : i(n) {}
};

template <A a> struct B { int i; constexpr B() : i(a.i) { } };

template<typename> void bar () {
    B<{1}> var;
}

void fu() {
    bar<int>();
}
