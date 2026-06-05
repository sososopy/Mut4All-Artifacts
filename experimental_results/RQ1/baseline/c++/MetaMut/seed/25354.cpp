

template < typename > struct A;
template < typename > struct B {};

template < typename T >
using C = typename A < T >::template D < T >;

template < typename T > struct A
{ 
  // should be: template < typename > struct D : B < C < T > > {};
  struct D : B < C < T > > {};
};

A < int > a;
