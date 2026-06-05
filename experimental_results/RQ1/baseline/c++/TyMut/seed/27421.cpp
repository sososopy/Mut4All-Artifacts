
template <typename T, typename U>
inline constexpr bool is_same_v = false;

template <typename T>
inline constexpr bool is_same_v<T, T> = true;

void f(float x, float& r) {
    [=] {
        static_assert(is_same_v<decltype(x), float>);
        static_assert(is_same_v<decltype((x)), float const&>);
        static_assert(is_same_v<decltype(r), float&>);
        static_assert(is_same_v<decltype((r)), float const&>); // error
    }();
}
