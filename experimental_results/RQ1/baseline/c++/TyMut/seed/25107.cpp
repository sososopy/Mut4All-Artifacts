


#include <array>
#include <cstddef>
#include <exception>

template <std::size_t N>
constexpr size_t simple_find(const std::array<int, N> &arg_array,
                             const int                &arg_value
                             ) {
	for (size_t ctr = 0; ctr != N; ++ctr) {
		if ( arg_array[ ctr ] == arg_value ) {
			return ctr;
		}
	}
	throw std::out_of_range( "" );
}

static constexpr std::array<int, 3> some_ints { { 10, 11 } };

static_assert( simple_find( some_ints, 10 ) == static_cast<size_t>( 0 ), "" );
static_assert( simple_find( some_ints, 11 ) == static_cast<size_t>( 1 ), "" );

int main() { }


