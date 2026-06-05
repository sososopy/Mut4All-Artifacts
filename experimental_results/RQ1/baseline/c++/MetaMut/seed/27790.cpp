
    template<typename T>
    constexpr bool is_integral_(...) {
        return false;
    }
    template<typename T, T = 1>
    constexpr bool is_integral_(long) {
        return true;
    }

    static_assert(is_integral_<int>(42));
    static_assert(!is_integral_<void>(42));

    struct S {};
    static_assert(!is_integral_<S>(42));

