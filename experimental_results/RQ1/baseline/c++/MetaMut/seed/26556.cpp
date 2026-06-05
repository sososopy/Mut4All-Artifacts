#include <boost/core/demangle.hpp>
#include <quadmath.h>
#include <typeinfo>
#include <iostream>
#include <string>

template <typename Type>
inline std::string nameof()
{
 return boost::core::demangle(typeid(Type).name());
}

int main()
{
 std::cout << nameof<decltype(HUGE_VALQ)>() << std::endl;
 std::cout << nameof<decltype(__builtin_nansq(""))>() << std::endl;
 std::cout << nameof<decltype(__builtin_nanq(""))>() << std::endl;
}

