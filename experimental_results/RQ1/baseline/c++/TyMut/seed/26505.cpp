
#include <compare>

int main()
{
    struct X {
        auto operator<=>(const X&) const = default;
    };
    X x;
    static_assert(noexcept(x < x));
    static_cast<void>(x < x);
}

