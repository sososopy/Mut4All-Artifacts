

template<int I> struct tag: tag<I-1> {};
template<> struct tag<0> {};

template<typename...>
struct B;

template<typename F, typename... O>
struct B<F, O...>: F, B<O...> {
    B(F f, O... o): F{f}, B<O...>(o...) {}
    using B<O...>::operator();
    template<typename... A>
    void operator()(tag<sizeof...(O)>, A... a) { F::operator()(a...); }
};

template<>
struct B<> { void operator()(); };

template<typename... F>
struct S: B<F...> {
    S(F... f): B<F...>{f...} {}
    template<typename... A>
    void operator()(A... a) { B<F...>::operator()(tag<sizeof...(F)>{}, a...); }
};

int main() {
    auto l1 = [](auto){};
    auto l2 = [](auto){};
    S<decltype(l1), decltype(l2)> s{l1, l2};
    s(42);
}
