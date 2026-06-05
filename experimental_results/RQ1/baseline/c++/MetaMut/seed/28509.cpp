#include <concepts>

template <std::integral T>
    requires std::unsigned_integral<T>
struct S
{
    template <std::integral T2> // template <typename T2>
    friend class S;
};

int main()
{
    S<unsigned> s;
}

