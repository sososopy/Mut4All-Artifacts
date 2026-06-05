
#include <vector>

template <typename t>
struct foo
{
    template <typename some_t>
    constexpr foo(t const it,
                  some_t const index,
                  size_t const my_index = 0) :
        m_it{it},
        m_index{index},
        m_my_index{my_index}
    {}

    t m_it{};
    size_t m_index{};
    size_t m_my_index{};
};

int main()
{
    using foo_t = decltype(foo{10, 4u, 2}); // works
    // using foo_ice_t = decltype(foo{10, 4u}); // ICE: type deduction in decltype seems to cause this?
    using foo_fine_t = decltype(foo<size_t>{10, 4u}); // explicit type declaration works fine.

    foo f{10, 3u}; // works fine out of decltype expression.
}

