

template < typename ... > struct X {};
template < typename ... T > int f (X < T, T ... > ...);
int a = f (X < int, int > ());
