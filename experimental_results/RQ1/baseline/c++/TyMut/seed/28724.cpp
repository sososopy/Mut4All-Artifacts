

#include <memory>

struct NonTrivial
{
    constexpr NonTrivial() : b(true) {}

    bool b;
};

union Un
{
    bool f1;
    NonTrivial f2;
};

constexpr bool test()
{
    Un un{ .f1 = false };
    un.f2 = {}; // should be an error here, and require std::construct_at(&un.f2);
    return un.f2.b;
}

static_assert(test());
