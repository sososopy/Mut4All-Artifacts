
#include <memory>
#include <span>
#include <ranges>
#include <concepts>

template <typename T> struct S;

template <typename C> constexpr bool is_S = false;
template <typename T> constexpr bool is_S<S<T>> = true;

template <typename T>
struct S {
    T data;

    S();
    explicit S(const T &);

    template <typename U,
              std::enable_if_t<!is_S<U>, bool> = false>
    requires std::equality_comparable_with<T, U>
    friend bool operator==(const S &s, const U &u) { return s.data == u; }
};

template <typename T>
bool operator==(const S<T> &a, const S<T> &b) { return a.data == b.data; }


int main() {
    using SD = S<double>;

    char data[sizeof(SD) * 10];
    std::span<SD> span((SD*)std::begin(data), (SD*)std::end(data));

    //static_assert(std::equality_comparable<std::span<SD>::iterator>);

    std::ranges::uninitialized_default_construct(span);
}

