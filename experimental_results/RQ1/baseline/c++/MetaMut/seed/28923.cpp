
template<typename T>
struct in_place_type_t {};

template<typename T>
inline constexpr in_place_type_t<T> in_place_type;

template<typename T, typename... Ts>
inline constexpr bool outer_helper_v = true;

template<typename... Ts>
struct foo {
    struct type;

    template<typename U>
    static constexpr bool helper_v = outer_helper_v<U, Ts...>;
};

template<typename... Ts>
struct foo<Ts...>::type {
    template<typename U>
        requires helper_v<U>
    type(in_place_type_t<U>) {}
};

int main() {
    foo<int>::type x(in_place_type<int>);
}
