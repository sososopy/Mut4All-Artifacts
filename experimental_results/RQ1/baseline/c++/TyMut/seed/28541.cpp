
#include <iostream>

struct Bar {
};

int main()
{
    Bar bar;
    constexpr bool same = requires
    {
        { bar } -> std::same_as<Bar>;
    };

    constexpr bool conv = requires
    {
        { bar } -> std::convertible_to<Bar>;
    };

    if constexpr (std::same_as<decltype(bar), Bar>)
    {
        std::cout << "AS SAME" << std::endl;
    }

    if constexpr (std::is_same_v<decltype(bar), Bar>)
    {
        std::cout << "IS SAME" << std::endl;
    }

    if constexpr (same)
    {
        std::cout << "REQUIRES SAME" << std::endl;
    }

    if constexpr (conv)
    {
        std::cout << "CONVERTIBLE" << std::endl;
    }

    return 0;
}
