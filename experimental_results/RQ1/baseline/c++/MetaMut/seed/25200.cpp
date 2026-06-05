

#include <type_traits>
#include <iostream>
using namespace std;

template <typename T>
struct Outer
{
    template <typename T2>
    struct Inter
    {
        constexpr static bool value = std::is_same<T2, T>::value;
    };

    template <typename T2>
    constexpr static bool value = Inter<T2>::value;
};

template <typename T1, typename T2>
struct Wrapper
{
    constexpr static bool value = Outer<T1>::template value<T2>;
};

int main()
{
    cerr << Wrapper<int, int>::value << endl;
}

