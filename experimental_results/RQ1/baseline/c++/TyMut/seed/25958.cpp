
#include <tuple>
struct A {};
constexpr auto tuple = std::tuple_cat( std::make_tuple( A(), A() ), std::make_tuple( A() ) );
