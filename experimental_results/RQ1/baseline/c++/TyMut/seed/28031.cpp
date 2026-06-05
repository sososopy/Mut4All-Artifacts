
#include <type_traits>

template<class X, void(X::*foo)() = nullptr>
struct p
{
    template<void(X::*fun)() = foo, typename T = std::enable_if_t<nullptr == fun, int>>
    p(T) { }
    p() = default;
};

struct A
{
    p<A> i = 1;
    void bar();
    p<A, &A::bar> j;
};
