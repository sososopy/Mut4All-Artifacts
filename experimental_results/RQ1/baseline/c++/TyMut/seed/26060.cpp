
#include <array>
#include <initializer_list>

int main()
{
    constexpr std::array<int, 3> a = {{ 1, 2, 3 }};
    constexpr std::initializer_list<int> b = { a[0], a[1], a[2] };

    return 0;
}

