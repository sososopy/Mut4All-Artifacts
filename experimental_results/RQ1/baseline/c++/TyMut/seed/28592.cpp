
#include <type_traits>

template <typename T>
using probably_unsigned = decltype([] {
    return std::make_unsigned_t<T>{};
}());

template <typename T>
auto definitely_unsigned_helper() {
    return std::make_unsigned_t<T>{};
}

template <typename T>
using definitely_unsigned = decltype(definitely_unsigned_helper<T>());


static_assert(std::is_same_v<probably_unsigned<char>, unsigned char>);
static_assert(std::is_same_v<probably_unsigned<short>, unsigned short>);

static_assert(std::is_same_v<definitely_unsigned<char>, unsigned char>);
static_assert(std::is_same_v<definitely_unsigned<short>, unsigned short>);

void no_template_no_problem()
{
    probably_unsigned<char> p;
    static_assert(std::is_same_v<decltype(p), unsigned char>);
}

template <typename T>
void definitely_no_problem()
{
    definitely_unsigned<T> d;
    static_assert(std::is_unsigned_v<decltype(d)>);
}

template <typename T>
void problem()
{
    probably_unsigned<T> p;
    static_assert(std::is_unsigned_v<decltype(p)>);

    // Always resolved as int!!!
    static_assert(std::is_same_v<int, decltype(p)>); // !!!!!!
}

int main()
{
    problem<char>();
    problem<short>();
    problem<long>();

    definitely_no_problem<char>();
    definitely_no_problem<short>();
    definitely_no_problem<long>();
}
