
#include <utility>

template< typename T, typename = std::make_index_sequence< T::size() > >
struct inclusive_scan;

template< std::size_t... Ns, std::size_t... Is >
struct inclusive_scan< std::index_sequence< Ns... >, std::index_sequence< Is... > >
{
    template< std::size_t I >
    using part = std::integral_constant< std::size_t, ( 0 + ... + ( ( Is <= I ) ? Ns : 0 ) ) >;

    using type = std::index_sequence< part< Is >::value... >;
};

int main()
{
    using S = std::index_sequence<3,1,2,1,3>;

    using IS = inclusive_scan<S>::type;
    static_assert( std::is_same_v<IS, std::index_sequence<3,4,6,7,10>> );
}
