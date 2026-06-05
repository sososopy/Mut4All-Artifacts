
#include <type_traits>

template< typename, typename = void >
struct has_const_int_dummy
  : std::false_type
{};

template< typename T >
struct has_const_int_dummy< T, typename std::enable_if< std::is_same< decltype( std::declval< T >().dummy ), const int >::value >::type >
  : std::true_type
{};

struct A0 { const int dummy; };
struct A1 {};
struct A2 { int dummy(); };
struct A3 { static const int dummy = 0; };

int main()
{
  static_assert( has_const_int_dummy< A0 >::value, "A0" );
  static_assert( !has_const_int_dummy< A1 >::value, "A1" );
  static_assert( !has_const_int_dummy< A2 >::value, "A2" );
  static_assert( !has_const_int_dummy< A3 >::value, "A3" ); // line 23
}
