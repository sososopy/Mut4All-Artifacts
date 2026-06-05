
#include <iostream>

template < std::size_t N >
struct A {  enum : std::size_t {  value = N  };  };

template < class... TL >
using B = A< sizeof...(TL) >;

template < class T >
struct C {  using type = T;  };

template < class... TL >
using D = typename C< B<TL...> >::type;

template < class T, class U, class... TL >
void foo( T, U, TL... )
{
    std::cout << (B<T, U, TL...>::value) << std::endl;
    std::cout << (D<T, U, TL...>::value) << std::endl;
    std::cout << (C< B<T, U, TL...> >::type::value) << std::endl;
}

int main()
{
    foo(10, 20, 30, 40, 50);
}

