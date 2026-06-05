
#include <stdint.h>
#include <tuple>
#include <string>

struct Cow{};
struct Dog{};
struct Horse{}; // of course

template <typename ... Ts>
struct meow{

    template<size_t Idx>
    decltype(auto)
    get()
    {
        if      constexpr (Idx == 0) { return Cow{}; }
        else if constexpr (Idx == 1) { return Dog{}; }
        else if constexpr (Idx == 2) { return Horse{}; }
    }
};

template <typename ... Ts>
meow<Ts...> kitty()
{
    return meow<Ts...>{};
}

namespace std
{
    template<typename ... Ts>
    struct tuple_size< meow<Ts...> >
    : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    template< std::size_t Idx, class... Ts >
    struct tuple_element< Idx, meow<Ts...> >
    {
        using type = decltype(  std::declval<  meow<Ts...>  >().template get<Idx>()  );
    };
}

int main(int argc, char const *argv[])
{
    auto && [ moo, woof, neigh, vv ] = kitty<Cow, Dog, Horse, void>();
}


