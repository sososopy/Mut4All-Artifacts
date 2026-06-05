
template< typename t, typename u >
void f( t, u ) = delete;

template< typename t >
void c( t o ) { f( o, 5 ); }

namespace n {
template< typename t >
void f( t, int ) {}

void p() { c( []{} ); }
}


namespace n {
struct s { struct n1 { struct n2 {}; }; };
int f( s::n1::n2 ) { return 3; }
}
int i = f( n::s::n1::n2() );
