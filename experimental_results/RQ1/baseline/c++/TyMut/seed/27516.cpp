
template <typename T, typename U> struct is_same { static constexpr bool value = false; };
template <typename T> struct is_same<T, T> { static constexpr bool value = true; };

template <typename T> using some_metafunction_t = T;

template <typename T>
void foo(T ) {
    using X = T*;
    using Y = some_metafunction_t<T>;

    static_assert(is_same<X, Y>::value);
}

void bar() {
    foo(0);
}

