
#include <compare>

struct C {
    int x[4];
    auto operator<=>(C const&) const = default;
};

bool check(C const& a, C const& b) {
    return (a <=> b) == 0;
}

