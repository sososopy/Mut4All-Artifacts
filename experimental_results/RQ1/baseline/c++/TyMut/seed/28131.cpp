
#include <array>
#include <type_traits>
#include <iostream>

template <typename T, std::size_t N>
struct A {
    A() = default;
    A(const A &) = default;
    A(A &&) = default;
    
    T arr[N];   
};

int main()
{
    std::cout << std::is_aggregate<A<int,3>>::value;
}
