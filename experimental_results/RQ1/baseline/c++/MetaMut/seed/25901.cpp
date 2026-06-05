#include <limits>
#include <memory>
#include <iostream>
#include <cxxabi.h>
#include <cstdlib>

template <typename T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

int
main()
{
    std::cout << std::numeric_limits<long long>::min() << '\n';
    std::cout << -9223372036854775808 << '\n';
    std::cout << static_cast<long long>(0x8000000000000000) << '\n';
    std::cout << type_name<decltype(-9223372036854775808)>() << '\n';
}

