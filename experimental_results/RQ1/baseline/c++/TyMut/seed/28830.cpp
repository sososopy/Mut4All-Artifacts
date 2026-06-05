
struct B1
{
    void foo(int) {}
};

struct B2
{
    void foo(bool) {}
};

struct D: B1
#ifndef ONE_BASE
, B2
#endif
{
    using B1::foo;
#ifndef ONE_BASE
    using B2::foo;
#endif
};

template <typename Class, typename Param>
constexpr auto select(void (Class::*func)(Param))
{
    return func;
}

int main()
{
    constexpr auto bFunc = select<B1, int>(&B1::foo);
    constexpr auto dFunc = select<D, int>(&D::foo);

    static_assert(bFunc == dFunc, "");
    
    constexpr auto bFuncCastToDFunc = static_cast<void (D::*)(int)>(bFunc);
    constexpr auto dFuncCastToBFunc = static_cast<void (B1::*)(int)>(dFunc);
}
