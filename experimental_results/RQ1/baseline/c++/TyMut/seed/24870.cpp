
#include <bit>

struct A {  signed char b:1 = 0; signed char b1:7 = 0; };

struct B { unsigned char b; };

static_assert( std::bit_cast<B>(A{}).b == 0 );
