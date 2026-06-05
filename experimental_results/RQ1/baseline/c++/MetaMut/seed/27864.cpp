
constexpr bool f0(int n) {
    if (__builtin_constant_p(n))
        return true;
    return false;
    // Alternatively:
    // return __builtin_constant_p(n) ? true : false;
    // return __builtin_constant_p(n);
}
constexpr bool f1(int n) {
    if constexpr (__builtin_constant_p(n))
        return true;
    return false;
}
static_assert( f0(1));
static_assert( f1(1)); // gcc 9.1 fails

