
template<int... Is>
struct int_sequence {};
template<int N>
struct S {
    template <int... Is>
    requires sizeof...(Is) == N
    int mf1(int_sequence<Is...>);
    void mf2() {
        mf1(int_sequence<>{});
    }
};

