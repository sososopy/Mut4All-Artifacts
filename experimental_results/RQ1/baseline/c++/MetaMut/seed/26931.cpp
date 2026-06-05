
#include <cstddef>
#include <type_traits>

template <typename T, std::size_t S>
constexpr std::size_t lengthof(const volatile T (&)[S])
{
    return S;
}

int main()
{
    volatile int meow[4];
    static_cast<void>(meow); // shut up warning
    return static_cast<int>(std::integral_constant<std::size_t,
        lengthof(meow)>::value);
}

