
template <typename F>
bool call(F&&) {
    typename F::Out var = false;
    return var;
}
bool test() {
    struct localc  {
        using Out = bool;
    };
    return call(localc{});
}
