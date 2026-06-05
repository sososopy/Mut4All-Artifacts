
void f() = delete;

template< typename t >
void ft() { f( t() ); }

template< typename t >
struct ct { decltype( f( t() ) ) m; };

