
#include <type_traits>

template< typename, typename = void >
struct A : std::false_type {};

template< typename T >
struct A< T, typename std::enable_if< !!sizeof( std::declval< T >() ) >::type > : std::true_type {};
//                               here ^^

struct B;

int main()
{
  static_assert( A< int >::value, "int" );
  static_assert( !A< B >::value, "B" );
}

