

template < typename T > struct A
{
  enum E : T;
  E h ();
};

A < int > a;
A < int >::E b = a.h ();

template < typename T > enum A < T >::E : T { e };

template < typename T > typename A < T >::E A < T >::h ()
{
  return e; 
}
