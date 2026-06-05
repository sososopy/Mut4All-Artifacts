struct A {
    template <typename M> static constexpr bool C = true;
    template <typename M> friend void f() requires (C<M>) {}
};

struct B {
    template <typename M> static constexpr bool C = false;
    template <typename M> friend void f() requires (C<M>) {}
};