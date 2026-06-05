#include <type_traits>

template<bool b>
struct X
{
    template<typename T>
    X(T);
};

template<bool b>
explicit(b) X(std::bool_constant<b>) -> X<b>;
