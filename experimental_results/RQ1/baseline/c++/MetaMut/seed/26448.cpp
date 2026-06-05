
#include <iostream>
#include <type_traits> 
#include <concepts>
template < typename T >
concept test = std::same_as <decltype(T::func), int(int) >;
struct D
{
    int func(int);
};
int main()
{ 
    std::cout << test<D>;      
}
