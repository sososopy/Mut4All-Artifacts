
template <class A, class B>
static constexpr bool C = true;

template <class... Ts>
struct Foo {
    template <class... Us>
        requires((... && C<Ts, Us>))
    void bar(const Foo<Us...>& r) {}

    template <class... Us>
        requires((... && C<Ts, Us>))
    friend void baz(const Foo& l, const Foo<Us...>& r) {}
};

int main() {
    Foo<int, int> x;

    x.bar(Foo<int, int>());  // Works.

    baz(x, Foo<int, int>());  // Doesn't work.
}
