
#include <array>

template< std::size_t N >
struct StrLiteral
{
    const std::array< char, N > _str;

    consteval StrLiteral( const char ( &a )[ N ] )
    : _str{ std::to_array( a ) }
    {}

    consteval auto operator == ( const StrLiteral & other ) const { return _str == other._str; }
};


const auto e = StrLiteral( "a" ) == StrLiteral( "b" );
