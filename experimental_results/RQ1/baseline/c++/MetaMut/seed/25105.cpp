

template < typename > struct A {  using type = int;  };

template < typename ... T > using TN = int[sizeof ... (T)];

template < typename ... U >
void foo (TN < int, U ..., int, typename U::type ... >&) {}

template void foo < A < int >, A < float > > (int (&)[6]);
