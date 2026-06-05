

template<typename>
struct S {
    template<typename T>
    friend constexpr auto f(S, S<T>) {
        return true;
    }

    template<typename T> requires false
    friend constexpr auto f(S, T) {
        return false;
    }

    template<typename T> requires false
    friend constexpr auto f(T, S) {
        return false;
    }
};

static_assert(f(S<int>{}, S<int>{}));


