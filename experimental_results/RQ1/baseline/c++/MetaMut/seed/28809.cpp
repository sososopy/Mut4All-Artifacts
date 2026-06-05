
#include <bit>
#include <cstddef>

namespace test {
template<int> using voidify = void;

template<std::size_t N>
struct obj_rep { unsigned char value[N]; };

template<class T>
concept constant_value_initializable =
    requires { typename voidify<(std::bit_cast<obj_rep<sizeof(T)>>(T()), 0)>; };
}

struct foo {
    int x;
};

struct bar {
    int x = -1;
};

#ifdef MAYBE_CWG1581
using my_void = test::voidify<(std::bit_cast<test::obj_rep<sizeof(bar)>>(bar()), 0)>;
#endif

static_assert(test::constant_value_initializable<int>);
static_assert(test::constant_value_initializable<foo>);
static_assert(test::constant_value_initializable<bar>); // It's buggy
static_assert(!test::constant_value_initializable<int*>);
// Correct: std::bit_cast doesn't perform constant evaluation when involving pointers

int main()
{
}

