
#include <array>
#include <utility>
#include <type_traits>

template <template <class, auto...> class Template>
struct base {
    template <template <class, auto...> class, class = void>
    struct is_same_template: std::false_type {};
    template <class Dummy>
    struct is_same_template<Template, Dummy>: std::true_type {};
    template <
        template <class, auto...> class X,
        class = std::enable_if_t<is_same_template<X>::value>
    >
    constexpr void function() const noexcept {};
};

template <template <class, auto...> class... Templates>
struct derived: base<Templates>... {
    using base<Templates>::function...;
};

int main(int, char**) {
    derived<std::array, std::integer_sequence> x;
    x.function<std::array>();
}
