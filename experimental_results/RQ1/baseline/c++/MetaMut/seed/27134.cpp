#include<variant>
#include<concepts>

template<typename ... Args> struct pack {};
template<typename ... Args> struct uv : std::false_type {};
template<typename ... Args> struct uv<std::variant<Args...>> {
    using type = pack<Args...>;
};

template <typename T, typename ... Args>
concept is_any_of_impl_4 = requires(pack<Args>) {
    requires (std::same_as<T, Args> || ...);
};

inline std::variant<int, double> v;

template <typename T>
requires is_any_of_impl_4<bool, uv<decltype(v)>::type>
void bar() {}

int main() {
    bar<int>();
}

