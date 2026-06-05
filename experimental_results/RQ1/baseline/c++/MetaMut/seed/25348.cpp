
#include <array>
#include <iostream>

int main()
{
    constexpr std::size_t size = 4096;

    struct S
    {
        float f;
        S() : f(0.0f) {}
    };

    std::array<S, size> a = {};  // <-- note aggregate initialization

    for (auto& e : a)
        std::cerr << e.f;

    return 0;
}

