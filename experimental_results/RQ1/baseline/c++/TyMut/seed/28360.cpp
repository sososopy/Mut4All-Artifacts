#include <type_traits>

template<class T>
constexpr bool use_func_v{};

struct func_obj
{
    template<class T>
        requires use_func_v<T>
    void operator()(T, T) const
    {}
};

constexpr bool bar{std::is_invocable_v<func_obj, char, char>};

