
#include <cassert>
#include <cstring>
#include <type_traits>

struct A
{
    A() = default;
    A(unsigned a, unsigned b) : data_(a + b) {}

private:
    unsigned data_;
};

static_assert(std::is_trivial_v<A>, "");
static_assert(std::is_trivially_copyable_v<A>, "");

A foo(unsigned x)
{
    A result;
    std::memcpy(&result, &x, sizeof(x));
    return result;
}

