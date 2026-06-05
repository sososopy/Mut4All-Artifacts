

template < typename > using A = int;

//OK: template < typename X > A < X > a; 
template < typename X > A < X >::a;
