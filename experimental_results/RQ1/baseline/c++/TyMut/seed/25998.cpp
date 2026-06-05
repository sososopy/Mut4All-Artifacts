#include <type_traits>

struct Thrower
{
    ~Thrower() noexcept(false) { throw 1; }
};

struct Explicit
{
    ~Explicit() {}

    Thrower t;
};
static_assert(!std::is_nothrow_destructible<Explicit>::value, "Explicit");

