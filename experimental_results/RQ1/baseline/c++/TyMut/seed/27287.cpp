
#include <type_traits>

template< typename, template< auto... > typename >
struct IsInstantationOf : std::false_type {};

template< template< auto... > typename Template, auto... Arg >
struct IsInstantationOf< Template< Arg... >, Template > : std::true_type {};

template< std::size_t N >
struct StrLiteral {

    constexpr StrLiteral( const char ( & )[ N ] ) {}
};

template< StrLiteral >
struct Template {};

int main()
{
    const auto a = StrLiteral( "a" );

    static_assert( IsInstantationOf< Template< a >, Template >::value );

    return 0;
}
