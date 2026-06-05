template <class T>
struct has2param
{
    typedef char yes;
    typedef struct{char n[2];} no;
    template <class U>
    static yes check(U u) requires(requires{[](auto u){auto [a, b] = u;}(u);});
    static no check(...);
    static constexpr bool y = sizeof(check(T())) == sizeof(yes);
};

struct point
{
    int x;
    int y;
};

int main()
{
    point p{1, 2};
    constexpr bool ans = has2param<decltype(p)>::y;
    return 0;
}
