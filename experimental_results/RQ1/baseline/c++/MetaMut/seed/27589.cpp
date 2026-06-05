
    template <int N>
    struct A {
        void g() requires (N == 3) {}
        void f() requires (N == 3) { g(); }
    };

    template struct A<2>;
