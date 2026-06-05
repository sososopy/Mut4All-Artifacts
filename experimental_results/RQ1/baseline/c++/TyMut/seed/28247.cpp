
#include <type_traits>

struct A {
    A(A &&) noexcept(false) = default;
};

static_assert(!std::is_nothrow_move_constructible_v<A>);
