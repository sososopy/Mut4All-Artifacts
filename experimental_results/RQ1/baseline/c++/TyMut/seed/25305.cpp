
#include <cassert>

struct A
{
    int v;
    constexpr A() noexcept : v(3) { assert(v != 0); }
};

int main() { }
