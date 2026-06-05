
template < typename ... Ts > struct A 
{
  template < Ts ..., typename ... Us > struct B {};
};

A <>::B < int > e;
