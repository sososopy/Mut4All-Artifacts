
#include <concepts>

template <typename T>
class A
{
    using type = int;

    template <typename U>
    requires std::same_as<type, U>
    void Foo();
};

template <typename T>
template <typename U>
requires std::same_as<typename A<T>::type, U>
void A<T>::Foo()
{
}

