
Seems like an especially pointless restriction. From 5.19/2, this would seem to work around the issue, and in fact does:

#include <cstdio>

struct S
{
    template<typename T>
    constexpr operator T() const
    {
        return T{};
    }
};

constexpr S s {};
constexpr auto x = false ? [](){std::printf("hello world\n");} : s;

int main()
{
    x();
}
