
#include <cassert>

constexpr bool is_zero(int& x) { return x == 0; }

constexpr void f()
{
    int& x = *new int{0};
    assert(is_zero(x));
    x = 1;
    assert(!is_zero(x));
    delete &x;
}

constexpr int x{([]() consteval { f(); }(), 0)};
