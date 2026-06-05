
#include <utility>
#include <type_traits>

template <typename t, typename u>
concept same_as = std::is_same_v<t, u>;

template <typename t>
concept foo = requires(t v)
{
    {v} -> same_as<t>;
};

static_assert(foo<int>); // gcc 9 / clang
static_assert(foo<int &>); // clang
static_assert(foo<int const &>); // clang
