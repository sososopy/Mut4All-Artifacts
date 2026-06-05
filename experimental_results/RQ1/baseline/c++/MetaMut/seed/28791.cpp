
#include <iostream>
#include <type_traits>
#include <variant>
#include <vector>
#include <unordered_map>
#include <string>

#define TEMPLATE_DECL_SUB \
    template <typename, typename, typename...> class Map, \
    class Other

#define TEMPLATE_DECL \
    template <TEMPLATE_DECL_SUB>

template <TEMPLATE_DECL class Value, TEMPLATE_DECL_SUB>
struct trait_impl {
    using value_t = Value<Map, Other>;
    using object_t = Map<Other, value_t>;
    using other_t = Other;
    using variant_t = std::variant<other_t, object_t *>;

    trait_impl(value_t const &) {}
};

template <typename T>
using trait = decltype(trait_impl{std::declval<T>()});

TEMPLATE_DECL
struct test_type {
    using trait_t = trait<test_type>;
    using other_t = typename trait<test_type>::other_t;
    using variant_t = typename trait_t::variant_t;
    other_t _val;

    static constexpr bool noex = std::is_nothrow_constructible_v<other_t, Other>;

    //test_type() noexcept(noex): _val() {} // Works fine with this line instead of the next.
    test_type() noexcept(std::is_nothrow_constructible_v<other_t, Other>): _val() {}
    test_type(test_type const& oth) : _val(oth._val) {}
};

int main(void)
{
    test_type<std::unordered_map, int> t;
    return 0;
}
