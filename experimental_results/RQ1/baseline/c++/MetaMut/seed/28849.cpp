
#include <concepts>
#include <utility>

template <typename T>
constexpr std::decay_t<T> decay_copy(T&& x) noexcept {
    return std::forward<T>(x);
}

void dummy() {
    int arr[] {1, 2, 1, 1};
    static_assert(requires {
        // ok:
        { arr } -> std::same_as<int(&)[4]>;
        // ok:
        { decay_copy(arr) } -> std::same_as<int*>;
        // but:
        { auto(arr) } -> std::same_as<int*>;
    });
}
