
#include <array>
#include <cstddef>

template <std::size_t Size>
constexpr std::array<int, Size> foo() noexcept
{
    std::array<int, Size> ret;
    return ret;
}

int main()
{
    foo<2>();
}
