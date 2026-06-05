
#include <type_traits>

template <auto V>
struct S {
    static constexpr bool is_const = std::is_const_v<decltype(V)>;
};

struct W {
    constexpr W(int i) : i(i) {}
    int i;
};

int main() {
    static_assert(!S<1>::is_const);  // pass
    static_assert(!S<W(2)>::is_const);  // fail
}
