
#include <iostream>
#include <tuple>

template <typename T> struct DefaultValue { const T value=0;};
template <> struct DefaultValue<std::string> { const std::string value="Uninitialized";};

template <typename ... Args>
using Zero = std::tuple<DefaultValue<Args> ...>;

template <typename ... Args>
void f(const Zero<Args ...> &t)
{
    std::cout << sizeof...(Args) << std::endl;
}

int main()
{
    Zero<int,double,double> T;
    f(T);
    // f<int,double,double>(T); // This works
}
