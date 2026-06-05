
#include <type_traits>

template< typename, template< auto > typename >
struct IsImplementationOf : std::false_type {};

template< template< auto > typename Template, auto Arg >
struct IsImplementationOf< Template< Arg >, Template > : std::true_type {};

template< typename T >
struct X {
    constexpr X( T ) {}
};

template< X > struct Y {};

int main()
{
    static_assert( IsImplementationOf< Y< X(0) >, Y > );

    return 0;
}
