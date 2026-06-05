
#include <cstdio>
template<typename T>
static void Use(T& plot)
{
    plot(1);
}
int main()
{
    static const int table[1] = {123456};
    Use([&](auto x)
    {
        unsigned var = table[x];
        unsigned ui = var;
        std::printf("%u\n", ui);
    });
}
