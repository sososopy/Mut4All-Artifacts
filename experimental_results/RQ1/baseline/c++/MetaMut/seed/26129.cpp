#include <array>
#include <tuple>

template<typename> struct Sizes;

template<typename ...Ts> struct Sizes<std::tuple<Ts...>> {
    static std::array<size_t, sizeof...(Ts)> sizes()
    {
        return { sizeof(Ts)... };
    }
};

size_t f(size_t i) {
    return Sizes<std::tuple<unsigned, bool>>::sizes()[i];
}

A