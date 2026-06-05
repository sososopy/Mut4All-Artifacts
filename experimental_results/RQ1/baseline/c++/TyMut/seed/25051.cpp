
template<typename T>
concept bool C = requires(T t) { t.i; };
template<C T>
auto f(T t) {
    return t.i;
}
struct S {
};
auto x = f(S{});

