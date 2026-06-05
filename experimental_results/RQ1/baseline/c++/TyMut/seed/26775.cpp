
#include <cassert>

struct Foo {
    constexpr Foo(const unsigned i) : val(i) {}
    constexpr Foo operator-(const Foo &rhs) const {
        return assert(val >= rhs.val), Foo(val - rhs.val);
    }
    unsigned val;
};

constexpr Foo foo(Foo(1) - Foo(2));
