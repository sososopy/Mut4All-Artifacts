
struct Traits {
    static constexpr bool value = true;
};

template<typename T, typename Traits>
concept cBarOf = Traits::value;

template<typename Traits, auto bar>
struct Foo {
    template<cBarOf<Traits> auto rhsBar> // <-- this concept causes ICE.
    void doNothing(Foo<Traits, rhsBar> const&);
};

void someFunction() {
    Foo<Traits, 0> lhs{};
    lhs.doNothing(lhs);
}
