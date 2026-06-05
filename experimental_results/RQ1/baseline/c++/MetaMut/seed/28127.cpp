
#include <type_traits>
#include <array>

template<typename T>
concept integral_constant_ = std::is_empty_v<T> && std::is_trivial_v<T> &&
requires
{
    typename T::value_type;
    requires std::is_integral_v<typename T::value_type>;
    { T::value } -> typename T::value_type;
};

struct sz_fn
{
template<typename R> requires requires(R&& r) { {static_cast<R&&>(r).size()} -> integral_constant_; }
constexpr auto operator()(R&& r) {
    return static_cast<R&&>(r).size();
}
};

constexpr auto sz = sz_fn{};


int main()
{
    auto arr = std::array<int, 5>{1, 2, 3, 4, 5};

    return !std::is_invocable_v<decltype(sz), decltype(arr)>;
}
