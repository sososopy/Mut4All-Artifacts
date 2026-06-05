
#include <compare>

struct S
{
    int s = 0;
    S(int s) : s(s) {}
    auto operator<=>(const S&) const = default;
};

struct T : S
{
    T(int s) : S(s) {}
    constexpr auto operator<=>(const T&) const = default;
};

int main() { return T(0) >= T(1); } // Expected 0, but returns 1
